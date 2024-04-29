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
        return 0;
    }
    uint16_t expectedParamBytes() { return 3; };
    RpcClass *returnObjectType() { return nullptr; };
};

class DriveController_SetSpeed : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        int16_t speed = (parameters[0] << 8) | parameters[1];
        ((DriveController *)bindTo)->setSpeed(speed);
        return 0;
    }
    uint16_t expectedParamBytes() { return 2; };
    RpcClass *returnObjectType() { return nullptr; };
};

class DriveController_SetAngle : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        int8_t angle = parameters[0];
        ((DriveController *)bindTo)->setAngle(angle);
        return 0;
    }
    uint16_t expectedParamBytes() { return 1; };
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
        return 2;
    }
    uint16_t expectedParamBytes() { return 0; };
    RpcClass *returnObjectType() { return nullptr; };
};

class DriveController_GetAngle : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        uint8_t val = ((DriveController *)bindTo)->getAngle();
        returnValue[0] = val & 0xff;
        return 1;
    }
    uint16_t expectedParamBytes() { return 0; };
    RpcClass *returnObjectType() { return nullptr; };
};

class DriveController_Pause : public RpcVoidFunction
{
public:
    void callFn(void *bindTo)
    {
        ((DriveController *)bindTo)->pause();
    }
};

class DriveController_Resume : public RpcVoidFunction
{
public:
    void callFn(void *bindTo)
    {
        ((DriveController *)bindTo)->resume();
    }
};

RpcClass *rpc_get_DriveControler()
{
    RpcClass *driveController = new RpcClass(8);
    driveController->addMember("drive", new DriveController_Drive);
    driveController->addMember("getSpeed", new DriveController_GetSpeed);
    driveController->addMember("setSpeed", new DriveController_SetSpeed);
    driveController->addMember("getAngle", new DriveController_GetAngle);
    driveController->addMember("setAngle", new DriveController_SetAngle);
    driveController->addMember("pause", new DriveController_Pause);
    driveController->addMember("resume", new DriveController_Resume);
    return driveController;
}