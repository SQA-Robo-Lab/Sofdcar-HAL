#ifndef RPC_CLASS_MEMBER_HPP
#define RPC_CLASS_MEMBER_HPP

#include "RpcClass.hpp"
#include "RpcChild.hpp"
#include "Arduino.h"

class RpcClassMember : public RpcChild
{
protected:
public:
    virtual void *call(void *bindTo, const char *parameters, char *returnValue) = 0;
    virtual uint16_t expectedParamBytes() = 0;
    virtual RpcClass *returnObjectType() = 0;
};

#endif