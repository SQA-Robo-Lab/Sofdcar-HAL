#ifndef RPC_ROOT_MEMBER_HPP
#define RPC_ROOT_MEMBER_HPP

#include "RpcClassMember.hpp"

class RpcRootMember : public RpcClassMember
{
protected:
    void *value;
    RpcClass *cls;

public:
    RpcRootMember(RpcClass *memberType, void *memberValue) : value(memberValue), cls(memberType) {}

    void *call(void *bindTo, const char *parameters, char *returnValue) { return this->value; };
    uint16_t expectedParamBytes() { return 0; };
    RpcClass *returnObjectType() { return this->cls; };

    void updateValue(void *newValue) { this->value = newValue; };
};

#endif