#ifndef RPC_MANAGER_HPP
#define RPC_MANAGER_HPP

#include "RpcClass.hpp"
#include "Arduino.h"

class RpcManager
{
protected:
    Stream &stream;
    RpcClass *classRoot;

    void call(const char *specification, uint16_t specLen);

public:
    RpcManager(Stream &dataStream, RpcClass *rpcClassRoot);

    void loop();
};

#endif