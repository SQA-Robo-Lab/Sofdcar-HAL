#include <Sofdcar-HAL.hpp>
#include <SimpleHardwareController.hpp>
#include <SimpleHardwareController_Connector.h>
#include "Config.hpp"
#include "Arduino.h"

SimpleHardwareController *controller = nullptr;

RpcClass *rootClass = nullptr;
RpcManager *mgr = nullptr;

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

    delay(100);
    Serial.println("Started");
}

void loop()
{
    mgr->loop();
    delay(100);
}
