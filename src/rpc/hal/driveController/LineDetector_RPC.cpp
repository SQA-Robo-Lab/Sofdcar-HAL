#include "../rpc_hal_header.hpp"
#include "../../RpcClassMember.hpp"
#include "../../../lineDetector/LineDetector.hpp"

class LineDetector_GetLegacyPosition : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        uint8_t pos = ((LineDetector *)bindTo)->getLegacyPosition();
        returnValue[0] = pos & 0xff;
        return 1;
    }
    uint16_t expectedParamBytes() { return 0; };
    RpcClass *returnObjectType() { return nullptr; };
};

class LineDetector_GetLinePositionMm : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        int8_t pos = ((LineDetector *)bindTo)->getLinePositionMm();
        returnValue[0] = pos & 0xff;
        return 1;
    }
    uint16_t expectedParamBytes() { return 2; };
    RpcClass *returnObjectType() { return nullptr; };
};

class LineDetector_GetLineAngle : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        int8_t angle = ((LineDetector *)bindTo)->getLineAngle();
        returnValue[0] = angle & 0xff;
        return 0;
    }
    uint16_t expectedParamBytes() { return 1; };
    RpcClass *returnObjectType() { return nullptr; };
};

/**
 * Changed signature: expecting 1 parameter (max size), returns array with size min(max size, 10)
 */
/*TODO
class LineDetector_GetAllDetectedLines : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        uint16_t val = ((LineDetector *)bindTo)->getAllDetectedLines();
        returnValue[0] = (val >> 8) & 0xff;
        returnValue[1] = val & 0xff;
        return 2;
    }
    uint16_t expectedParamBytes() { return 0; };
    RpcClass *returnObjectType() { return nullptr; };
};*/

RpcClass *rpc_get_LineDetector()
{
    RpcClass *lineDetector = new RpcClass(4);
    lineDetector->addMember("getLegacyPosition", new LineDetector_GetLegacyPosition);
    lineDetector->addMember("getLinePositionMm", new LineDetector_GetLinePositionMm);
    lineDetector->addMember("getLineAngle", new LineDetector_GetLineAngle);
    // lineDetector->addMember("getAllDetectedLines", new LineDetector_GetAllDetectedLines);
    return lineDetector;
}