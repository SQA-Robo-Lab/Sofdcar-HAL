#ifndef RPC_CHILD_HPP
#define RPC_CHILD_HPP

enum RpcChild_Type
{
    RPC_CHILD_FUNCTION,
    RPC_CHILD_CLASS
};

class RpcChild
{
public:
    virtual RpcChild_Type type() = 0;
};

#endif