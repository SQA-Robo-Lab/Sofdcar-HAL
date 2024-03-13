#ifndef RPC_CLASS_HPP
#define RPC_CLASS_HPP

#include "Arduino.h"

class RpcClass
{
protected:
public:
    virtual void registerOperations() = 0;
};

#endif