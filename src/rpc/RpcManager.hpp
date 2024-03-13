#ifndef RPC_MANAGER_HPP
#define RPC_MANAGER_HPP

#include "Arduino.h"

class RpcManager
{
protected:
    Stream &stream;

public:
    RpcManager(Stream &stream);

    void loop();
};

#endif