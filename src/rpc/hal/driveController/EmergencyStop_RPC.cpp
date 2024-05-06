#include "../rpc_hal_header.hpp"
#include "../../RpcClassMember.hpp"
#include "../EmergencyStop.hpp"

class EmergencyStop_Alive : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        ((EmergencyStop *)bindTo)->alive();
        return 0;
    }
    uint16_t expectedParamBytes() { return 0; };
    RpcClass *returnObjectType() { return nullptr; };
};

class EmergencyStop_SetState : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        ((EmergencyStop *)bindTo)->setState(parameters[0] == 1 || parameters[0] == '1');
        return 0;
    }
    uint16_t expectedParamBytes() { return 1; };
    RpcClass *returnObjectType() { return nullptr; };
};

RpcClass *rpc_get_EmergencyStop()
{
    RpcClass *emergencyStop = new RpcClass(2);
    emergencyStop->addMember("alive", new EmergencyStop_Alive);
    emergencyStop->addMember("setState", new EmergencyStop_SetState);

    return emergencyStop;
}