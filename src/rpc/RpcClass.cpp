#define RPC_CLASS_DEBUG 1

#include "RpcClass.hpp"

RpcClass::RpcClass(uint16_t childCapacity) : numChildren(childCapacity)
{
    this->names = new char *[childCapacity];
    this->children = new RpcChild *[childCapacity];
    memset(this->names, 0, childCapacity * sizeof(this->names[0]));
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
        if (this->names[i] == nullptr)
        {
            this->names[i] = new char[strlen(name) + 1];
            strcpy(this->names[i], name);
            this->children[i] = child;
#ifdef RPC_CLASS_DEBUG
            Serial.print("Stored new member ");
            Serial.print(this->names[i]);
            Serial.print(" in slot ");
            Serial.println(i);
#endif
            return;
        }
    }
#ifdef RPC_CLASS_DEBUG
    Serial.println("Could not store new class member");
#endif
}

RpcChild *RpcClass::getMember(const char *name)
{
    for (uint16_t i = 0; i < this->numChildren; i++)
    {
        if (strcmp(name, this->names[i]) == 0)
        {
#ifdef RPC_CLASS_DEBUG
            Serial.print("Found class member ");
            Serial.println(this->names[i]);
#endif
            return this->children[i];
        }
    }
#ifdef RPC_CLASS_DEBUG
    Serial.println("Class member not found");
#endif
    return nullptr;
}