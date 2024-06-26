#include "../rpc_hal_header.hpp"
#include "../../RpcClassMember.hpp"
#include "../../../distanceSensor/DistanceSensor.hpp"

class DistanceSensor_GetDistanceToClosest : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        uint8_t dist = ((DistanceSensor *)bindTo)->getDistanceToClosestMm();
        returnValue[0] = (dist >> 8) & 0xff;
        returnValue[1] = dist & 0xff;
        return 2;
    }
    uint16_t expectedParamBytes() { return 0; };
    RpcClass *returnObjectType() { return nullptr; };
};

class DistanceSensor_GetAngleToClosest : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        int8_t angle = ((DistanceSensor *)bindTo)->getAngleToClosest();
        returnValue[0] = angle & 0xff;
        return 1;
    }
    uint16_t expectedParamBytes() { return 2; };
    RpcClass *returnObjectType() { return nullptr; };
};

RpcClass *rpc_get_DistanceSensor()
{
    RpcClass *distanceSensor = new RpcClass(2);
    distanceSensor->addMember("getDistanceToClosestMm", new DistanceSensor_GetDistanceToClosest);
    distanceSensor->addMember("getAngleToClosest", new DistanceSensor_GetAngleToClosest);
    return distanceSensor;
}