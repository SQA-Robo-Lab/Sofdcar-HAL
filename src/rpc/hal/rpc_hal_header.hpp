#ifndef RPC_HAL_HEADER_HPP
#define RPC_HAL_HEADER_HPP

#include "../RpcClass.hpp"

extern RpcClass *rpc_get_DistanceSensor();
extern RpcClass *rpc_get_DriveController();
extern RpcClass *rpc_get_EmergencyStop();
extern RpcClass *rpc_get_LineDetector();
extern RpcClass *rpc_get_SimpleHardwareController(RpcClass *driveControllerCls);

#endif