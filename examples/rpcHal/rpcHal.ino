#include <Sofdcar-HAL.hpp>
#include <SimpleHardwareController.hpp>
#include <SimpleHardwareController_Connector.h>
#include "Config.hpp"
#include "Arduino.h"

#include "WifiDebug.hpp"
WifiDebug *debug;

SimpleHardwareController *controller = nullptr;

RpcClass *rootClass = nullptr;
RpcManager *mgr = nullptr;

void onWifiMessage(char *data, uint16_t len)
{
    mgr->call(data, len);
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting");

    rootClass = rpc_createForHal();

    mgr = new RpcManager(Serial, rootClass);

    controller = new SimpleHardwareController;

    initSofdcarHalConnectorFor(controller);
    controller->initializeCar(config, lineConfig);

    ((RpcRootMember *)rootClass->getMember("dc"))->updateValue(controller->getDriveController());

    debug = new WifiDebug();
    debug->setOnMessage(onWifiMessage);

    delay(100);
    Serial.println("Started");
}

void loop()
{
    mgr->loop();
    delay(100);
}
