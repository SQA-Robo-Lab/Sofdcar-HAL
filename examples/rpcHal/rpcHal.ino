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

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting");

    Serial1.begin(115200);

    rootClass = rpc_createForHal();

    mgr = new RpcManager(Serial1, rootClass);

    controller = new SimpleHardwareController;

    initSofdcarHalConnectorFor(controller);
    controller->initializeCar(config, lineConfig);

    dc = controller->getDriveController();
    ((RpcRootMember *)rootClass->getMember("dc"))->updateValue(dc);
    ((RpcRootMember *)rootClass->getMember("stop"))->updateValue(&stop);

    /*debug = new WifiDebug();
    debug->setOnMessage(onWifiMessage);*/

    delay(100);
    Serial.println("Started");
}

void loop()
{
    mgr->loop();
    if (stop.check())
    {
        Serial.println("EMERGENCY STOP!!");
        dc->pause();
    }
    // delay(100);
}
