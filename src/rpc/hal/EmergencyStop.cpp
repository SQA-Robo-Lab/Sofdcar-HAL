#include "EmergencyStop.hpp"
#include "Arduino.h"

#define EMERGENCY_STOP_TIMEOUT 1000

void EmergencyStop::alive()
{
    Serial.println("Connection alive");
    this->lastAlive = max(millis(), 1);
}

bool EmergencyStop::check()
{
    if (this->lastAlive != 0 && millis() - lastAlive > EMERGENCY_STOP_TIMEOUT)
    {
        return true;
    }
    return false;
}

void EmergencyStop::setState(bool enabled)
{
    if (enabled)
    {
        this->alive();
    }
    else
    {
        this->lastAlive = 0;
    }
}