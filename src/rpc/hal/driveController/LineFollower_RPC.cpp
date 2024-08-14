#include "../rpc_hal_header.hpp"
#include "../../RpcClassMember.hpp"
#include "../../../lineFollower/LineFollower.hpp"

class LineFollower_SetLineToFollow : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        uint8_t line = parameters[0];
        ((LineFollower *)bindTo)->setLineToFollow(line);
        return 0;
    }
    uint16_t expectedParamBytes() { return 1; };
    RpcClass *returnObjectType() { return nullptr; };
};

class LineFollower_GetLineToFollow : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        uint8_t mode = ((LineFollower *)bindTo)->getLineToFollow();
        returnValue[0] = mode & 0xff;
        return 1;
    }
    uint16_t expectedParamBytes() { return 0; };
    RpcClass *returnObjectType() { return nullptr; };
};

class LineFollower_SetMode : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        LineFollowMode mode = (LineFollowMode)parameters[0];
        ((LineFollower *)bindTo)->setMode(mode);
        return 0;
    }
    uint16_t expectedParamBytes() { return 1; };
    RpcClass *returnObjectType() { return nullptr; };
};

class LineFollower_GetMode : public RpcClassMember
{
public:
    void *call(void *bindTo, const char *parameters, char *returnValue)
    {
        uint8_t mode = ((LineFollower *)bindTo)->getMode();
        returnValue[0] = mode & 0xff;
        return 1;
    }
    uint16_t expectedParamBytes() { return 0; };
    RpcClass *returnObjectType() { return nullptr; };
};

RpcClass *rpc_get_LineFollower()
{
    RpcClass *lineFollower = new RpcClass(4);
    lineFollower->addMember("setLineToFollow", new LineFollower_SetLineToFollow);
    lineFollower->addMember("getLineToFollow", new LineFollower_GetLineToFollow);
    lineFollower->addMember("setMode", new LineFollower_SetMode);
    lineFollower->addMember("getMode", new LineFollower_GetMode);
    return lineFollower;
}