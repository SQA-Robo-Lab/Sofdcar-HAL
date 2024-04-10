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
        uint16_t nameLen = index - nameStart;
        char *childName = new char[nameLen + 1];
        memcpy(childName, specification + index, nameLen);
        childName[nameLen] = 0;

        RpcClassMember *member = (RpcClassMember *)currClass->getMember(childName);
        delete[] childName;

        if (specification[index] != '(')
        {
            // error: Was function but no brackets found
            return;
        }
        index++;

        uint16_t expectedBytes = member->expectedParamBytes();
        if (index + expectedBytes >= specLen || specification[index + expectedBytes] != ')')
        {
            // error: Function call not erminated with ) after expected number of bytes
            return;
        }
        RpcClass *returnClass = member->returnObjectType();
        if (returnClass != nullptr)
        {
            if (specification[index + expectedBytes + 1] != '.')
            {
                // error: cannot termiate rpc call with function that still returns object
                return;
            }
            currObj = member->call(currObj, specification + index, nullptr);
            if (currObj == nullptr)
            {
                // error: Function call returned null, no further call possible
                return;
            }
            currClass = returnClass;
        }
        else
        {
            if (index + expectedBytes + 1 < specLen)
            {
                // error: Must terminate rpc call after function that does not return object
                return;
            }
            member->call(currObj, specification + index, /*todo: capture return*/ nullptr);
        }
    }
}

void RpcManager::loop()
{
    this->call("", 0);
}