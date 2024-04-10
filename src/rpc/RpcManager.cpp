#include "RpcManager.hpp"

#include "RpcClassMember.hpp"

RpcManager::RpcManager(Stream &dataStream, RpcClass *rpcClassRoot) : stream(dataStream), classRoot(rpcClassRoot)
{
}

void RpcManager::call(const char *specification, uint16_t specLen)
{
    void *currObj = nullptr;
    RpcClass *currClass = this->classRoot;
    uint16_t index = 0;
    while (index < specLen)
    {
        uint16_t nameStart = index;
        for (; index < specLen && specification[index] != '(' && specification[index] != '.'; index++)
        {
        }
        uint16_t nameLen = (index - nameStart) * sizeof(specification[0]);
        char *childName = new char[nameLen + 1];
        memcpy(childName, specification + nameStart, nameLen);
        childName[nameLen] = 0;

        RpcClassMember *member = (RpcClassMember *)currClass->getMember(childName);
        delete[] childName;

        if (member == nullptr)
        {
            // error: member with specified name not found
            this->stream.println("error: member with specified name not found");
            return;
        }

        RpcClass *returnClass = member->returnObjectType();
        uint16_t expectedBytes = member->expectedParamBytes();

        bool hasOpeningBracket = false;
        if (expectedBytes > 0 && specification[index] != '(')
        {
            // error: Was function but no brackets found
            this->stream.println("error: Was function but no brackets found");
            return;
        }
        else if (specification[index] == '(')
        {
            // member call uses brackets => move index to next dot/end
            index += 1 + expectedBytes + 1;
            hasOpeningBracket = true;
        }

        if (hasOpeningBracket && specification[index - 1] != ')')
        {
            // error: Function call not terminated with ) after expected number of bytes
            this->stream.println("error: Function call not terminated with ) after expected number of bytes");
            return;
        }
        if (returnClass != nullptr)
        {
            if (specification[index] != '.')
            {
                // error: cannot termiate rpc call with function that still returns object
                this->stream.println("error: cannot termiate rpc call with function that still returns object");
                return;
            }
            currObj = member->call(currObj, specification + index - expectedBytes - (hasOpeningBracket ? 1 : 0), nullptr);
            if (currObj == nullptr)
            {
                // error: Function call returned null, no further call possible
                this->stream.println("error: Function call returned null, no further call possible");
                return;
            }
            currClass = returnClass;
        }
        else
        {
            if (index < specLen)
            {
                // error: Must terminate rpc call after function that does not return object
                this->stream.println("error: Must terminate rpc call after function that does not return object");
                return;
            }
            char returnVal[1024];
            memset(returnVal, 0, 1024);
            member->call(currObj, specification + index - expectedBytes - (hasOpeningBracket ? 1 : 0), returnVal);
            this->stream.print("return:");
            this->stream.println(returnVal);
        }
        index++;
    }
}

void RpcManager::loop()
{
    char buffer[1024];
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
    }
}