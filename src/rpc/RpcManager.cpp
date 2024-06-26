#define RPC_MANAGER_DEBUG 1

#define MAX_CMD_LEN 512
#define MAX_EXECUTION_MODIFIER_LEN 10
#define TIMEOUT 500
#define MAX_SUBSCRIPTIONS 10

#include "RpcManager.hpp"
#include "RpcClassMember.hpp"

bool isAlphaNum(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

RpcManager::RpcManager(Stream &dataStream, RpcClass *rpcClassRoot) : stream(dataStream), classRoot(rpcClassRoot), streamRead(dataStream)
{
    this->commandPart = new char[MAX_CMD_LEN];
    this->executionModifier = new char[MAX_EXECUTION_MODIFIER_LEN];
    this->subscriptions = new RpcSubscription *[MAX_SUBSCRIPTIONS];
    this->resetCall();
}

RpcManager::~RpcManager()
{
    delete[] this->commandPart;
    delete[] this->executionModifier;

    for (uint16_t i = 0; i < this->subscriptionIndex; i++)
    {
        delete this->subscriptions[i];
    }
    delete[] this->subscriptions;
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
    this->executionModifierType = RPC_EXECUTION_TYPE_REGULAR;
}

void RpcManager::error(const char *errorText, const char *methodName, Readable &readable)
{
#ifdef RPC_MANAGER_DEBUG
    Serial.print("Command error: ");
    Serial.println(errorText);
#endif
    this->stream.write((int8_t)(strlen(errorText) + strlen(methodName) + 7));
    this->stream.print("error:");
    this->stream.print(methodName);
    this->stream.print(":");
    this->stream.println(errorText);
    this->resetCall();
    while (readable.available())
    {
        readable.read();
    }
    return;
}

void RpcManager::successReturn(const char *fnName, const char *data, uint16_t dataLen)
{
    this->stream.write((int8_t)(dataLen + 8 + strlen(fnName)));
    this->stream.print("return:");
    this->stream.print(fnName);
    this->stream.print(":");
    this->stream.write(data, dataLen);
    this->resetCall();
}

RpcManager_ExecutionModifierType RpcManager::getExecutionModifierType(const char *mod, uint16_t len)
{
    if (mod[len - 1] != ':' && len < MAX_EXECUTION_MODIFIER_LEN)
    {
        return RPC_EXECUTION_TYPE_UNKNOWN;
    }
    if (len == 7 && mod[1] == 's' && mod[2] == 'u' && mod[3] == 'b')
    {
        return RPC_EXECUTION_TYPE_SUBSCRIPTION;
    }

    if (len >= MAX_EXECUTION_MODIFIER_LEN)
    {
        return RPC_EXECUTION_TYPE_REGULAR;
    }
    else
    {
        return RPC_EXECUTION_TYPE_UNKNOWN;
    }
}

void RpcManager::callInternal(Readable &readable)
{
    bool timeoutReached = false;
#ifdef TIMEOUT
    unsigned long now = millis();
    if (this->lastCommandPart > 0 && this->commandIndex > 0 && now - this->lastCommandPart > TIMEOUT)
    {
        timeoutReached = true;
        Serial.println("Command timeout reached");
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
        Serial.print("Continuing call with ");
        Serial.print(this->commandIndex);
        Serial.print(", ");
        Serial.println(this->parametersStart);
#endif
        if (this->parametersStart == 0)
        {
#ifdef RPC_MANAGER_DEBUG
            Serial.println("Reading member name");
#endif
            for (; this->commandPart[this->commandIndex - 1] != '(' && this->commandPart[this->commandIndex - 1] != '.' && this->commandIndex < MAX_CMD_LEN;)
            {
                if (readable.available())
                {
                    char c = readable.read();
                    if (this->executionModifierType == RPC_EXECUTION_TYPE_UNKNOWN)
                    { // currently reading execution modifier
                        this->executionModifier[this->commandIndex++] = c;

                        this->executionModifierType = this->getExecutionModifierType(this->executionModifier, this->commandIndex);
                        if (this->executionModifierType != RPC_EXECUTION_TYPE_UNKNOWN)
                        { // full execution modifier has been read => reset command read index
                            this->executionModifier[this->commandIndex] = 0;
                            this->commandIndex = 0;
#ifdef RPC_MANAGER_DEBUG
                            Serial.print("Received execution modifier ");
                            Serial.println(this->executionModifierType);
#endif
                        }
                    }
                    else if (isAlphaNum(c) || c == '(' || c == '.')
                    { // reading regular command part
                        this->commandPart[this->commandIndex++] = c;
                    }
                    else if (this->commandIndex == 0 && c == ':' && this->executionModifierType == RPC_EXECUTION_TYPE_REGULAR)
                    { // found start of execution modifier, no execution modifier captured yet
                        this->executionModifier[this->commandIndex++] = c;
                        this->executionModifierType = RPC_EXECUTION_TYPE_UNKNOWN;
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
        Serial.print("Searching member ");
        Serial.println(this->commandPart);
#endif

        RpcClassMember *member = (RpcClassMember *)currClass->getMember(this->commandPart);

        if (member == nullptr)
        {
            // error: member with specified name not found
            this->error("member with specified name not found", commandPart, readable);
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
            this->error("Was function but no brackets found", commandPart, readable);
            return;
        }
        else if (this->commandPart[this->parametersStart - 1] == '(')
        {
// member call uses brackets => read number of expected bytes until next dot/end
#ifdef RPC_MANAGER_DEBUG
            Serial.println("Reading parameters");
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
                this->error("Function call not terminated with ) after expected number of bytes", commandPart, readable);
                return;
            }
        }

#ifdef RPC_MANAGER_DEBUG
        Serial.print("Len of parameters ");
        Serial.println(this->commandIndex - this->parametersStart);
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
                this->error("cannot termiate rpc call with function that still returns object", commandPart, readable);
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
                this->error("Function call returned null, no further call possible", commandPart, readable);
                return;
            }
            currClass = returnClass;
            this->commandIndex = 0;
            this->parametersStart = 0;
            if (timeoutReached)
            {
                // error: timeout reached and no other command error
                this->error("Command timeout reached", "", readable);
                return;
            }
        }
        else
        {
            if (this->commandPart[this->commandIndex - 1] == '.' || (readable.available() && readable.peek() == '.'))
            {
                // error: Must terminate rpc call after function that does not return object
                this->error("Must terminate rpc call after function that does not return object", commandPart, readable);
                return;
            }
            switch (this->executionModifierType)
            {
            case RPC_EXECUTION_TYPE_REGULAR:
            {
                char returnVal[MAX_CMD_LEN];
                memset(returnVal, 0, MAX_CMD_LEN);
                int16_t numBytes = (int16_t)(member->call(currObj, commandPart + parametersStart, returnVal));
#ifdef RPC_MANAGER_DEBUG
                Serial.print("Command success. Return value ");
                Serial.println(returnVal);
#endif
                this->successReturn(commandPart, returnVal, numBytes);
                break;
            }
            case RPC_EXECUTION_TYPE_SUBSCRIPTION:
            {
                int32_t existingIndex = -1;
                for (uint16_t i = 0; i < this->subscriptionIndex; i++)
                {
                    if (this->subscriptions[i]->matches(member, currObj))
                    {
                        existingIndex = i;
                        break;
                    }
                }
                if (existingIndex < 0 && this->subscriptionIndex >= MAX_SUBSCRIPTIONS)
                {
                    this->error("Maximum number of subscriptions exeeded", commandPart, readable);
                    return;
                }
                uint16_t updateRate = ((this->executionModifier[4] << 8) & 0xff00) | (this->executionModifier[5] & 0xff);
                if (existingIndex < 0)
                {
                    this->subscriptions[this->subscriptionIndex++] = new RpcSubscription(member, currObj, commandPart + parametersStart, expectedBytes, commandPart, updateRate);
#ifdef RPC_MANAGER_DEBUG
                    Serial.print("Added subscription with update rate ");
                    Serial.println(updateRate);
#endif
                }
                else
                {
                    this->subscriptions[existingIndex]->setUpdateRate(updateRate);
#ifdef RPC_MANAGER_DEBUG
                    Serial.print("Updated existing subscription to update rate ");
                    Serial.println(updateRate);
#endif
                }
                this->stream.write((int8_t)(12 + strlen(commandPart)));
                this->stream.print(":sub:return:");
                this->stream.print(commandPart);
                this->resetCall();
                break;
            }
            default:
            {
                this->error("Unknown execution modifier specified", commandPart, readable);
                return;
            }
            }
        }
        timeoutReached = false;
    }
}

void RpcManager::loop()
{
    this->callInternal(this->streamRead);
    for (uint16_t i = 0; i < this->subscriptionIndex; i++)
    {
        this->subscriptions[i]->loop(&RpcManager::successReturn, this);
    }
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