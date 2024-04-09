#include "RpcClass.hpp"

RpcClass::RpcClass(uint16_t childCapacity) : numChildren(childCapacity)
{
    this->names = new char *[childCapacity];
    this->children = new RpcChild *[childCapacity];
}

RpcClass::~RpcClass()
{
    delete[] this->names;
    delete[] this->children;
}

void RpcClass::addChild(const char *name, RpcChild *child)
{
    for (uint16_t i = 0; i < this->numChildren; i++)
    {
        if (this->children[i] == nullptr)
        {
            this->names[i] = name;
            this->children[i] = child;
            return;
        }
    }
}

RpcChild *RpcClass::subField(const char *name)
{
    for (uint16_t i = 0; i < this->numChildren; i++)
    {
        if (strcmp(name, this->names[i]))
        {
            return this->children[i];
        }
    }
    return nullptr;
}