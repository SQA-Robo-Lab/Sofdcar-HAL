#ifndef RPC_MANAGER_HPP
#define RPC_MANAGER_HPP

#include "RpcClass.hpp"
#include "Readable.hpp"
#include "Arduino.h"

class RpcManager
{
protected:
    Stream &stream;
    RpcClass *classRoot;
    ReadableStream streamRead;

    void *currObj;
    RpcClass *currClass;
    char *commandPart;
    uint16_t commandIndex;
    uint16_t parametersStart;
    unsigned long lastCommandPart;

    void callInternal(Readable &readable);
    void error(const char *errorText, Readable &readable);
    void resetCall();

public:
    RpcManager(Stream &dataStream, RpcClass *rpcClassRoot);
    ~RpcManager();

    void call(const char *specification, uint16_t specLen);
    void loop();
};

#endif