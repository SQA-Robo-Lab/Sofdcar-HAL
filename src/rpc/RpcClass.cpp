#include "RpcClass.hpp"

RpcClass::RpcClass(uint16_t childCapacity) : numChildren(childCapacity)
{
    this->names = new char *[childCapacity];
    this->children = new RpcChild *[childCapacity];
}

RpcClass::~RpcClass()
{
    for (uint16_t i = 0; i < this->numChildren; i++)
    {
        if (this->names[i] != nullptr)
        {
            delete[] this->names[i];
        }
    }
    delete[] this->names;
    delete[] this->children;
}

void RpcClass::addMember(const char *name, RpcChild *child)
{
    for (uint16_t i = 0; i < this->numChildren; i++)
    {
        if (this->children[i] == nullptr)
        {
            this->names[i] = new char[strlen(name) + 1];
            strcpy(this->names[i], name);
            this->children[i] = child;
            return;
        }
    }
}

RpcChild *RpcClass::getMember(const char *name)
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