#include "RpcHal.hpp"
#include "../RpcRootMember.hpp"
#include "./driveController/DriveController_RPC.cpp"

RpcClass *rpc_createForHal()
{
    RpcClass *driveControllerCls = rpc_get_DriveControler();

    RpcClass *root = new RpcClass(1);
    root->addMember("dc", new RpcRootMember(driveControllerCls, nullptr /*todo: add actual instance*/));
    return root;
}