#ifndef RPC_CLASS_MEMBER_HPP
#define RPC_CLASS_MEMBER_HPP

#include "RpcClass.hpp"
#include "RpcChild.hpp"
#include "Arduino.h"

class RpcClassMember : public RpcChild
{
protected:
public:
    /**
     * @return The pointer to the object returned or the number of bytes written to "returnValue" if returnObjectType == nullptr
     */
    virtual void *call(void *bindTo, const char *parameters, char *returnValue) = 0;
    virtual uint16_t expectedParamBytes() = 0;
    virtual RpcClass *returnObjectType() = 0;
};

class RpcVoidFunction : public RpcClassMember
{
protected:
    virtual void callFn(void *bindTo);

public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        this->callFn(bindTo);
        return 0;
    };
    uint16_t expectedParamBytes()
    {
        return 0;
    };
    RpcClass *returnObjectType()
    {
        return nullptr;
    };
};

#endif