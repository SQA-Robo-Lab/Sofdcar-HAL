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

    /**
     * Expected specification format: :mod:x.y(--).z()
     * Parts:
     * - `:mod:` (optional) Execution modifier, changes how the call is executed. Possible values:0
     *     - `:sub--:` Subscribe to the call (execute it automatically on a regular basis) where `--` is a uint16_t specifying the update rate
     *         - 0=when value changes; Not recommended for values that are time consuming to check (e.g. ultrasonic) or change often
     * - `x.`/`y(--).`/`z()`: Function/Member calls
     *     - For Functions/members without parameters: Brackets optional; recommended to add brackets to last function call to speed up parsing
     *     - With parameters: Brackets requires, Parameters in binary, big endian as required by function
     * - Return:
     *     - `error:z:message` on error (z is name of trailing function call, message is an error message string)
     *     - `return:z:--` on success (z is name of trailing function call, -- is return data in binary as specified by called function)
     */
    void call(const char *specification, uint16_t specLen);
    void loop();
};

#endif