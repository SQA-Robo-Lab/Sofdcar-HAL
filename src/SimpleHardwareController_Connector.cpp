#include "SimpleHardwareController_Connector.h"
#include "SimpleHardwareController.hpp"

//--- Housekeeping and c++ wrapping functions ---
SimpleHardwareController *currentInstance;
void initSofdcarHalConnectorFor(SimpleHardwareController *instance)
{
    currentInstance = instance;
}

//--- C compatibility functions ---

int16_t SimpleHardwareController_DriveController_GetSpeed()
{
    return currentInstance->getDriveController()->getSpeed();
}

int16_t SimpleHardwareController_DriveController_GetNonPausedSpeed()
{
    return currentInstance->getDriveController()->getNonPausedSpeed();
}
int8_t SimpleHardwareController_DriveController_GetAngle()
{
    return currentInstance->getDriveController()->getAngle();
}
void SimpleHardwareController_DriveController_SetSpeed(int16_t speed)
{
    return currentInstance->getDriveController()->setSpeed(speed);
}
void SimpleHardwareController_DriveController_SetAngle(int8_t angle)
{
    return currentInstance->getDriveController()->setAngle(angle);
}
void SimpleHardwareController_DriveController_Pause()
{
    return currentInstance->getDriveController()->pause();
}
void SimpleHardwareController_DriveController_Resume()
{
    return currentInstance->getDriveController()->resume();
}

int8_t SimpleHardwareController_LineDetector_GetLinePositionMm()
{
    return currentInstance->getLineDetector()->getLinePositionMm();
}
int8_t SimpleHardwareController_LineDetector_GetLineAngle()
{
    return currentInstance->getLineDetector()->getLineAngle();
}

uint16_t SimpleHardwareController_DistanceSensor_GetDistanceToClosestMm(uint8_t sensorIndex)
{
    return currentInstance->getDistanceSensor(sensorIndex)->getDistanceToClosestMm();
}
int8_t SimpleHardwareController_DistanceSensor_GetAngleToClosestMm(uint8_t sensorIndex)
{
    return currentInstance->getDistanceSensor(sensorIndex)->getAngleToClosest();
}

void SimpleHardwareController_LineFollower_Loop()
{
    return currentInstance->getLineFollower()->loop();
}
void SimpleHardwareController_LineFollower_SetLineToFollow(uint8_t lineIndex)
{
    return currentInstance->getLineFollower()->setLineToFollow(lineIndex);
}