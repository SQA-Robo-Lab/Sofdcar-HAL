#include "Arduino.h"
#include <Sofdcar-HAL.hpp>

uint8_t speed, speedBuffer, motorNum = 255, directionBuffer, i2cSendBufferIndex = 255;
char i2cSendBuffer[32];

void motorDebugLoop(DriveController &dc)
{
}