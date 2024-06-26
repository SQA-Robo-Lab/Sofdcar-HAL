#include "../rpc_hal_header.hpp"
#include "../../RpcClassMember.hpp"
#include "../../../SimpleHardwareController.hpp"

class SimpleHardwareController_GetLineFollower : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        return ((SimpleHardwareController *)bindTo)->getLineFollower();
    }
    uint16_t expectedParamBytes() { return 0; };
    RpcClass *returnObjectType() { return nullptr; /*todo*/ };
};

class SimpleHardwareController_GetDistanceSensor : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        ((SimpleHardwareController *)bindTo)->getDistanceSensor(parameters[0]);
        return 0;
    }
    uint16_t expectedParamBytes() { return 1; };
    RpcClass *returnObjectType() { return nullptr; /*todo*/ };
};

class SimpleHardwareController_GetNumDistanceSensors : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        uint8_t val = ((SimpleHardwareController *)bindTo)->getNumDistanceSensors();
        returnValue[0] = val & 0xff;
        return 1;
    }
    uint16_t expectedParamBytes() { return 1; };
    RpcClass *returnObjectType() { return nullptr; };
};

class SimpleHardwareController_GetLineDetector : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        return ((SimpleHardwareController *)bindTo)->getLineDetector();
    }
    uint16_t expectedParamBytes() { return 0; };
    RpcClass *returnObjectType() { return nullptr; /*todo*/ };
};

class SimpleHardwareController_GetDriveController : public RpcClassMember
{
protected:
    RpcClass *cls;

public:
    SimpleHardwareController_GetDriveController(RpcClass *driveControllerCls) : cls(driveControllerCls) {}
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        return ((SimpleHardwareController *)bindTo)->getDriveController();
    }
    uint16_t expectedParamBytes() { return 0; };
    RpcClass *returnObjectType() { return nullptr; };
};

RpcClass *rpc_get_SimpleHardwareController(RpcClass *driveControllerCls)
{
    RpcClass *simpleHardwareController = new RpcClass(5);
    simpleHardwareController->addMember("getDriveController", new SimpleHardwareController_GetLineFollower);
    simpleHardwareController->addMember("getLineDetector", new SimpleHardwareController_GetDistanceSensor);
    simpleHardwareController->addMember("getNumDistanceSensors", new SimpleHardwareController_GetNumDistanceSensors);
    simpleHardwareController->addMember("getDistanceSensor", new SimpleHardwareController_GetLineDetector);
    simpleHardwareController->addMember("getLineFollower", new SimpleHardwareController_GetDriveController(driveControllerCls));
    return simpleHardwareController;
}