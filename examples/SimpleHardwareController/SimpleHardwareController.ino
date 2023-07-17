#include <SimpleHardwareController.hpp>
#include "Config.hpp"

SimpleHardwareController controller;

void setup()
{
    controller.initializeCar(config, lineConfig);
    controller.getDriveController()->setSpeed(60);
}

void loop()
{
    controller.loop();
}