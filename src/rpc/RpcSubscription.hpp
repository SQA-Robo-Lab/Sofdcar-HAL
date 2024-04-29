#ifndef RPC_SUBSCRIPTION
#define RPC_SUBSCRIPTION

#include "RpcClassMember.hpp"
#include "Arduino.h"

#define RPC_SUBSCRIPTION_ON_CHANGE 0

class RpcManager;

class RpcSubscription
{
protected:
    RpcClassMember *member;
    void *obj;
    char *parameters;
    uint16_t updateRate = RPC_SUBSCRIPTION_ON_CHANGE;
    void *lastDataOrTime = nullptr;
    uint16_t lastDataSize = 0;
    char *fnName;

    bool needsToSend();

public:
    RpcSubscription(RpcClassMember *subscribeTo, void *bindTo, const char *callParameters, uint16_t parameterLen, const char *memberName, uint16_t sendUpdateRate = RPC_SUBSCRIPTION_ON_CHANGE);
    ~RpcSubscription();

    void loop(void (RpcManager::*successReturn)(const char *fnName, const char *data, uint16_t dataLen), RpcManager *instance);

    bool matches(RpcClassMember *subscribeTo, void *bindTo);
    void setUpdateRate(uint16_t sendUpdateRate);
};

#endif