#include "../rpc_hal_header.hpp"
#include "../../RpcClassMember.hpp"
#include "../../../driveController/DriveController.hpp"

class DriveController_Drive : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        int8_t angle = parameters[0];
        int16_t speed = (parameters[1] << 8) | parameters[2];
        ((DriveController *)bindTo)->drive(angle, speed);
        return nullptr;
    }
    uint16_t expectedParamBytes() { return 3; };
    RpcClass *returnObjectType() { return nullptr; };
};

class DriveController_GetSpeed : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        uint16_t val = ((DriveController *)bindTo)->getSpeed();
        returnValue[0] = (val >> 8) & 0xff;
        returnValue[1] = val & 0xff;
        return nullptr;
    }
    uint16_t expectedParamBytes() { return 0; };
    RpcClass *returnObjectType() { return nullptr; };
};

RpcClass *rpc_get_DriveControler()
{
    RpcClass *driveController = new RpcClass(8);
    driveController->addMember("drive", new DriveController_Drive);
    driveController->addMember("getSpeed", new DriveController_GetSpeed);
    return driveController;
}