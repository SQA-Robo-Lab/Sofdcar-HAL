#include <SimpleHardwareController.hpp>
#include "Config.hpp"
#include "Functions.h"

SimpleHardwareController controller;

void setup()
{
    Serial.begin(115200);
    controller.initializeCar(config, lineConfig);
    controller.getDriveController()->setSpeed(60);
    controller.getLineFollower()->setLineToFollow(1);
}

void loop()
{
    controller.loop();
}

int16_t getCurrentSpeed()
{
    return controller.getDriveController()->getSpeed();
}