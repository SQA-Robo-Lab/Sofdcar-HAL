#ifndef RPC_CLASS_HPP
#define RPC_CLASS_HPP

#include "RpcChild.hpp"
#include "Arduino.h"

class RpcFunction : public RpcChild
{
protected:
public:
    RpcChild_Type type() { return RPC_CHILD_FUNCTION; }

    virtual void call(void *bindTo, const char *parameters, char *returnValue) = 0;
};

#endif