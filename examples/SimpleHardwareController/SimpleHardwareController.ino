#include <SimpleHardwareController.hpp>
#include <SimpleHardwareController_Connector.h>
#include "Config.hpp"

#include "Test.h"

SimpleHardwareController controller;

void setup()
{
    Serial.begin(115200);
    initSofdcarHalConnectorFor(&controller);
    controller.initializeCar(config, lineConfig);
    controller.getDriveController()->setSpeed(60);
    controller.getLineFollower()->setLineToFollow(1);
}

void loop()
{
    controller.loop();
    testFn();
}