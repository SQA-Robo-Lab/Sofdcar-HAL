#define RPC_MANAGER_DEBUG 1

#define MAX_CMD_LEN 512
#define TIMEOUT 500

#include "RpcManager.hpp"
#include "RpcClassMember.hpp"

bool isAlphaNum(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

RpcManager::RpcManager(Stream &dataStream, RpcClass *rpcClassRoot) : stream(dataStream), classRoot(rpcClassRoot), streamRead(dataStream)
{
    this->commandPart = new char[MAX_CMD_LEN];
    this->resetCall();
}

RpcManager::~RpcManager()
{
    delete[] this->commandPart;
}

void RpcManager::call(const char *specification, uint16_t specLen)
{
    ReadableString readable(specification, specLen);
    this->callInternal(readable);
    this->resetCall();
}

void RpcManager::resetCall()
{
    this->currObj = nullptr;
    this->currClass = this->classRoot;
    this->commandIndex = 0;
    this->parametersStart = 0;
    this->lastCommandPart = 0;
}

void RpcManager::error(const char *errorText, Readable &readable)
{
    this->stream.print("error:");
    this->stream.println(errorText);
    this->resetCall();
    while (readable.available())
    {
        readable.read();
    }
    return;
}

void RpcManager::callInternal(Readable &readable)
{
    bool timeoutReached = false;
#ifdef TIMEOUT
    unsigned long now = millis();
    if (this->lastCommandPart > 0 && this->commandIndex > 0 && now - this->lastCommandPart > TIMEOUT)
    {
        timeoutReached = true;
    }
#endif
    while (readable.available() || timeoutReached)
    {
#ifdef TIMEOUT
        if (!timeoutReached)
        {
            this->lastCommandPart = now;
        }
#endif
#ifdef RPC_MANAGER_DEBUG
        this->stream.print("Continuing call with ");
        this->stream.print(this->commandIndex);
        this->stream.print(", ");
        this->stream.println(this->parametersStart);
#endif
        if (this->parametersStart == 0)
        {
#ifdef RPC_MANAGER_DEBUG
            this->stream.println("Reading member name");
#endif
            for (; this->commandPart[this->commandIndex - 1] != '(' && this->commandPart[this->commandIndex - 1] != '.' && this->commandIndex < MAX_CMD_LEN;)
            {
                if (readable.available())
                {
                    char c = readable.read();
                    if (isAlphaNum(c) || c == '(' || c == '.')
                    {
                        this->commandPart[this->commandIndex++] = c;
                    }
                }
                else
                {
                    if (timeoutReached)
                    {
                        break;
                    }
                    else
                    {
                        return; // wait for more data of command
                    }
                }
            }
            if (this->commandPart[this->commandIndex - 1] == '.' || this->commandPart[this->commandIndex - 1] == '(')
            {
                this->commandPart[this->commandIndex] = this->commandPart[this->commandIndex - 1];
                this->commandPart[this->commandIndex - 1] = 0;
                this->commandIndex++;
            }
            else
            {
                this->commandPart[this->commandIndex] = 0;
            }
        }

#ifdef RPC_MANAGER_DEBUG
        this->stream.print("Searching member ");
        this->stream.println(this->commandPart);
#endif

        RpcClassMember *member = (RpcClassMember *)currClass->getMember(this->commandPart);

        if (member == nullptr)
        {
            // error: member with specified name not found
            this->error("member with specified name not found", readable);
            return;
        }

        RpcClass *returnClass = member->returnObjectType();
        uint16_t expectedBytes = member->expectedParamBytes();

        if (this->parametersStart == 0)
        {
            this->parametersStart = this->commandIndex;
        }

        if (expectedBytes > 0 && this->commandPart[this->parametersStart - 1] != '(')
        {
            // error: Was function but no brackets found
            this->error("Was function but no brackets found", readable);
            return;
        }
        else if (this->commandPart[this->parametersStart - 1] == '(')
        {
// member call uses brackets => read number of expected bytes until next dot/end
#ifdef RPC_MANAGER_DEBUG
            this->stream.println("Reading parameters");
#endif
            for (uint16_t i = (this->commandIndex - this->parametersStart); i < expectedBytes + 1; i++)
            {
                if (readable.available())
                {
                    this->commandPart[this->commandIndex++] = readable.read();
                }
                else
                {
                    if (timeoutReached)
                    {
                        break;
                    }
                    else
                    {
                        return; // wait for more data of command
                    }
                }
            }
            if (this->commandPart[this->commandIndex - 1] != ')' || this->commandIndex - this->parametersStart != expectedBytes + 1)
            {
                // error: Function call not terminated with ) after expected number of bytes
                this->error("Function call not terminated with ) after expected number of bytes", readable);
                return;
            }
        }

#ifdef RPC_MANAGER_DEBUG
        this->stream.print("Len of parameters ");
        this->stream.println(this->commandIndex - this->parametersStart);
#endif

        if (returnClass != nullptr)
        {
            if (this->commandPart[this->commandIndex - 1] != '.' && (!readable.available() || readable.peek() != '.'))
            {
                if (!readable.available())
                {
                    if (!timeoutReached)
                    {
                        return; // wait for more data of command
                    }
                }
                // error: cannot termiate rpc call with function that still returns object
                this->error("cannot termiate rpc call with function that still returns object", readable);
                return;
            }
            else if (this->commandPart[this->commandIndex - 1] != '.')
            { // read remaining dot to prepare for next command part
                readable.read();
            }
            currObj = member->call(currObj, commandPart + parametersStart, nullptr);
            if (currObj == nullptr)
            {
                // error: Function call returned null, no further call possible
                this->error("Function call returned null, no further call possible", readable);
                return;
            }
            currClass = returnClass;
            this->commandIndex = 0;
            this->parametersStart = 0;
            if (timeoutReached)
            {
                // error: timeout reached and no other command error
                this->error("Command timeout reached", readable);
                return;
            }
        }
        else
        {
            if (this->commandPart[this->commandIndex - 1] == '.' || (readable.available() && readable.peek() == '.'))
            {
                // error: Must terminate rpc call after function that does not return object
                this->error("Must terminate rpc call after function that does not return object", readable);
                return;
            }
            char returnVal[MAX_CMD_LEN];
            memset(returnVal, 0, MAX_CMD_LEN);
            member->call(currObj, commandPart + parametersStart, returnVal);
            this->stream.print("return:");
            this->stream.println(returnVal);
            this->resetCall();
        }
        timeoutReached = false;
    }
}

void RpcManager::loop()
{
    this->callInternal(this->streamRead);
    /*char buffer[1024];
    memset(buffer, 0, 1024);
    uint16_t bufPos = -1;
    while (this->stream.available() > 0)
    {
        buffer[++bufPos] = this->stream.read();
        if (buffer[bufPos] == '\n' || bufPos >= 1023)
        {
            this->stream.print("Executing ");
            buffer[bufPos] = 0;
            this->stream.println(buffer);
            this->call(buffer, bufPos);
            memset(buffer, 0, bufPos + 1);
            bufPos = 0;
        }
    }*/
}