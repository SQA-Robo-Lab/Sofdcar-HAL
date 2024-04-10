#include "RpcHal.hpp"
#include "../RpcRootMember.hpp"
#include "./rpc_hal_header.hpp"

RpcClass *rpc_createForHal()
{
    RpcClass *driveControllerCls = rpc_get_DriveControler();

    RpcClass *root = new RpcClass(1);
    root->addMember("dc", new RpcRootMember(driveControllerCls, nullptr /*todo: add actual instance*/));
    return root;
}