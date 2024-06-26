#include "RpcHal.hpp"
#include "../RpcRootMember.hpp"
#include "./rpc_hal_header.hpp"

#include "Arduino.h"

class TmpTestFn : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        int16_t value = ((parameters[0] << 8) & 0xff00) | parameters[1];
        Serial.print("Received: ");
        Serial.println(value);
        returnValue[0] = parameters[0];
        returnValue[1] = parameters[1];
        return nullptr;
    }
    uint16_t expectedParamBytes() { return 2; };
    RpcClass *returnObjectType() { return nullptr; };
};

RpcClass *rpc_createForHal()
{
    RpcClass *driveControllerCls = rpc_get_DriveController();
    RpcClass *simpleHardwareControllerCls = rpc_get_SimpleHardwareController(driveControllerCls);
    RpcClass *lineDetectorCls = rpc_get_LineDetector();
    RpcClass *distanceSensorCls = rpc_get_DistanceSensor();

    RpcClass *root = new RpcClass(6);
    root->addMember("dc", new RpcRootMember(driveControllerCls, nullptr));
    root->addMember("shc", new RpcRootMember(simpleHardwareControllerCls, nullptr));
    root->addMember("ld", new RpcRootMember(lineDetectorCls, nullptr));
    root->addMember("ds", new RpcRootMember(distanceSensorCls, nullptr));
    root->addMember("test", new TmpTestFn);
    root->addMember("stop", new RpcRootMember(rpc_get_EmergencyStop(), nullptr));
    return root;
}