#include "../rpc_hal_header.hpp"
#include "../../RpcClassMember.hpp"
#include "../../../driveController/DriveController.hpp"

class DriveController_Drive : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        int8_t angle = parameters[0];
        int16_t speed = (parameters[1] << 16) | parameters[2];
        ((DriveController *)bindTo)->drive(angle, speed);
        return nullptr;
    }
    uint16_t expectedParamBytes() { return 3; };
    RpcClass *returnObjectType() { return nullptr; };
};

RpcClass *rpc_get_DriveControler()
{
    RpcClass driveController(8);
    driveController.addMember("drive", new DriveController_Drive);
}