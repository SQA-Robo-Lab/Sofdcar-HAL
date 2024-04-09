#include <Sofdcar-HAL.hpp>
#include "Arduino.h"

RpcManager mgr(Serial, rpc_createForHal());

void setup()
{
}

void loop()
{
    mgr.loop();
}