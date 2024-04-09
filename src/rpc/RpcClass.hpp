#ifndef RPC_CLASS_HPP
#define RPC_CLASS_HPP

#include "RpcChild.hpp"
#include "Arduino.h"

class RpcClass : public RpcChild
{
protected:
    const char **names;
    RpcChild **children;
    uint16_t numChildren;

public:
    RpcChild_Type type() { return RPC_CHILD_CLASS; }

    RpcClass(uint16_t childCapacity);
    ~RpcClass();

    void addChild(const char *name, RpcChild *child);

    /**
     * @param name CATUION: Prefix-Matching
     */
    RpcChild *subField(const char *name);
};

#endif