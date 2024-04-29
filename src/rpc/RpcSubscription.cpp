#include "RpcSubscription.hpp"

#define MAX_CMD_LEN 512

RpcSubscription::RpcSubscription(
    RpcClassMember *subscribeTo,
    void *bindTo,
    const char *callParameters,
    uint16_t parameterLen,
    const char *memberName,
    uint16_t sendUpdateRate = RPC_SUBSCRIPTION_ON_CHANGE) : member(subscribeTo),
                                                            obj(bindTo),
                                                            updateRate(sendUpdateRate)
{
    uint16_t fnNameLen = strlen(memberName);
    this->fnName = new char[fnNameLen];
    strcpy(fnName, memberName);
    if (parameterLen > 0)
    {
        this->parameters = new char[parameterLen];
        memcpy(this->parameters, callParameters, parameterLen);
    }
    else
    {
        this->parameters = nullptr;
    }

    if (this->updateRate != RPC_SUBSCRIPTION_ON_CHANGE)
    {
        this->lastDataOrTime = new unsigned long;
    }
}

RpcSubscription::~RpcSubscription()
{
    delete[] this->fnName;
    if (this->parameters != nullptr)
    {
        delete[] this->parameters;
    }

    if (this->updateRate == RPC_SUBSCRIPTION_ON_CHANGE)
    {
        if (this->lastDataOrTime != nullptr)
        {
            delete[] this->lastDataOrTime;
        }
    }
    else
    {
        delete this->lastDataOrTime;
    }
}

bool RpcSubscription::needsToSend()
{
    if (this->lastDataOrTime == nullptr)
    {
        return true;
    }
    else if (this->updateRate != RPC_SUBSCRIPTION_ON_CHANGE)
    {
        unsigned long now = millis();
        unsigned long lastTime = *((unsigned long *)this->lastDataOrTime);
        if (now - lastTime >= this->updateRate)
        {
            *((unsigned long *)this->lastDataOrTime) = now;
            return true;
        }
        return false;
    }
    else
    {
        return true;
    }
}

void RpcSubscription::loop(void (RpcManager::*successReturn)(const char *fnName, const char *data, uint16_t dataLen), RpcManager *instance)
{
    if (this->needsToSend())
    {
        char returnValue[MAX_CMD_LEN];
        int16_t numBytes = (int16_t)(this->member->call(this->obj, this->parameters, returnValue));

        if (this->updateRate == RPC_SUBSCRIPTION_ON_CHANGE)
        {
            bool isSame = (numBytes == this->lastDataSize);
            if (this->lastDataOrTime != nullptr && isSame)
            {
                for (uint16_t i = 0; i < numBytes; i++)
                {
                    if (returnValue[i] != ((char *)this->lastDataOrTime)[i])
                    {
                        isSame = false;
                        break;
                    }
                }
            }
            if (isSame || numBytes == 0)
            {
                return;
            }
            else
            {
                if (this->lastDataSize < numBytes)
                {
                    delete[] this->lastDataOrTime;
                    this->lastDataOrTime = new char[numBytes];
                    this->lastDataSize = numBytes;
                }
                memcpy(this->lastDataOrTime, returnValue, numBytes);
            }
        }

        (instance->*(successReturn))(this->fnName, returnValue, numBytes);
    }
}