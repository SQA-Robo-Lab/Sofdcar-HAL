#ifndef RPC_MANAGER_HPP
#define RPC_MANAGER_HPP

#include "RpcClass.hpp"
#include "Readable.hpp"
#include "RpcSubscription.hpp"
#include "Arduino.h"

enum RpcManager_ExecutionModifierType
{
    RPC_EXECUTION_TYPE_UNKNOWN,
    RPC_EXECUTION_TYPE_REGULAR,
    RPC_EXECUTION_TYPE_SUBSCRIPTION
};

class RpcManager
{
protected:
    Stream &stream;
    RpcClass *classRoot;
    ReadableStream streamRead;

    void *currObj;
    RpcClass *currClass;
    char *commandPart;
    uint16_t commandIndex = 0;
    uint16_t parametersStart = 0;
    unsigned long lastCommandPart;
    char *executionModifier;
    RpcManager_ExecutionModifierType executionModifierType = RPC_EXECUTION_TYPE_REGULAR;

    RpcSubscription **subscriptions;
    uint16_t subscriptionIndex = 0;

    void callInternal(Readable &readable);
    void error(const char *errorText, const char *methodName, Readable &readable);
    void resetCall();
    void successReturn(const char *fnName, const char *data, uint16_t dataLen);
    RpcManager_ExecutionModifierType getExecutionModifierType(const char *mod, uint16_t len);

public:
    RpcManager(Stream &dataStream, RpcClass *rpcClassRoot);
    ~RpcManager();

    void call(const char *specification, uint16_t specLen);
    void loop();
};

#endif