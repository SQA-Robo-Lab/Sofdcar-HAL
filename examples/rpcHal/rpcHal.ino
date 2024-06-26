#include <Sofdcar-HAL.hpp>
#include <Sofdcar-RPC.hpp>
#include <SimpleHardwareController.hpp>
#include <SimpleHardwareController_Connector.h>
#include "Config.hpp"
#include "Arduino.h"

/*#include "WifiDebug.hpp"
WifiDebug *debug;*/

SimpleHardwareController *controller = nullptr;
DriveController *dc;
EmergencyStop stop;

RpcClass *rootClass = nullptr;
RpcManager *mgr = nullptr;

/*void onWifiMessage(char *data, uint16_t len)
{
    mgr->call(data, len);
}*/

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char *sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif // __arm__

int freeMemory()
{
    char top;
#ifdef __arm__
    return &top - reinterpret_cast<char *>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
    return &top - __brkval;
#else  // __arm__
    return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif // __arm__
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting");
    Serial.println(freeMemory());

    Serial1.begin(115200);

    rootClass = rpc_createForHal();

    mgr = new RpcManager(Serial1, rootClass);

    Serial.println(freeMemory());

    controller = new SimpleHardwareController;

    initSofdcarHalConnectorFor(controller);
    controller->initializeCar(config, lineConfig);

    Serial.println(freeMemory());

    dc = controller->getDriveController();
    ((RpcRootMember *)rootClass->getMember("dc"))->updateValue(dc);
    ((RpcRootMember *)rootClass->getMember("shc"))->updateValue(controller);
    ((RpcRootMember *)rootClass->getMember("ds"))->updateValue(controller->getDistanceSensor());
    ((RpcRootMember *)rootClass->getMember("stop"))->updateValue(&stop);

    Serial.println(freeMemory());

    /*debug = new WifiDebug();
    debug->setOnMessage(onWifiMessage);*/

    delay(100);
    Serial.println("Started");
}

void loop()
{
    mgr->loop();
    controller->loop();
    if (stop.check())
    {
        Serial.println("EMERGENCY STOP!!");
        dc->pause();
    }
    // delay(100);
}
