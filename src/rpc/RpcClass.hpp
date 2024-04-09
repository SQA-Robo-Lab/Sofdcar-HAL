#ifndef RPC_CLASS_HPP
#define RPC_CLASS_HPP

#include "RpcClassMember.hpp"
#include "Arduino.h"

class RpcClass
{
protected:
    char **names;
    RpcClassMember **children;
    uint16_t numChildren;

public:
    RpcClass(uint16_t childCapacity);
    ~RpcClass();

    void addMember(const char *name, RpcClassMember *child);

    RpcClassMember *getMember(const char *name);
};

#endif