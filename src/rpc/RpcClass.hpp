#ifndef RPC_CLASS_HPP
#define RPC_CLASS_HPP

#include "RpcChild.hpp"
#include "Arduino.h"

class RpcClass
{
protected:
    char **names;
    RpcChild **children;
    uint16_t numChildren;

public:
    RpcClass(uint16_t childCapacity);
    ~RpcClass();

    void addMember(const char *name, RpcChild *child);

    RpcChild *getMember(const char *name);
};

#endif