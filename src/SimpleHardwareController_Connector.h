#ifndef SOFDCAR_HAL_SIMPLE_HARDWARE_CONTROLLER_CONNECTOR_H
#define SOFDCAR_HAL_SIMPLE_HARDWARE_CONTROLLER_CONNECTOR_H

#include <Arduino.h>

#ifdef __cplusplus

#include "SimpleHardwareController.hpp"
void initSofdcarHalConnectorFor(SimpleHardwareController *instance);

extern "C"
{
#endif

    int16_t SimpleHardwareController_DriveController_GetSpeed();
    int16_t SimpleHardwareController_DriveController_GetNonPausedSpeed();
    int8_t SimpleHardwareController_DriveController_GetAngle();
    void SimpleHardwareController_DriveController_SetSpeed(int16_t speed);
    void SimpleHardwareController_DriveController_SetAngle(int8_t angle);
    void SimpleHardwareController_DriveController_Pause();
    void SimpleHardwareController_DriveController_Resume();

    int8_t SimpleHardwareController_LineDetector_GetLinePositionMm();
    int8_t SimpleHardwareController_LineDetector_GetLineAngle();

    uint16_t SimpleHardwareController_DistanceSensor_GetDistanceToClosestMm(uint8_t sensorIndex);
    int8_t SimpleHardwareController_DistanceSensor_GetAngleToClosestMm(uint8_t sensorIndex);

    void SimpleHardwareController_LineFollower_Loop();
    void SimpleHardwareController_LineFollower_SetLineToFollow(uint8_t lineIndex);

#ifdef __cplusplus
}
#endif

#endif