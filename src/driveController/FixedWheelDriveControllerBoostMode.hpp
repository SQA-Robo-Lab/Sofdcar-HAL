#ifndef FIXED_WHEEL_DRIVE_CONTROLLER_BOOST_MODE_HPP
#define FIXED_WHEEL_DRIVE_CONTROLLER_BOOST_MODE_HPP

#include "FixedWheelDriveController.hpp"
#include "../motors/Motor.hpp"

class FixedWheelDriveControllerBoostMode : public FixedWheelDriveController
{
private:
    unsigned long lastUpdate;
    uint16_t requiredSpeed = 0;
    int16_t lastSpeed = 0;
    int8_t lastAngle = 0;

public:
    FixedWheelDriveControllerBoostMode(
        Motor &frontLeftMotor, Motor &frontRightMotor,
        Motor &rearLeftMotor, Motor &rearRightMotor,
        uint8_t wheelDistanceWidthMm, uint8_t wheelDistanceLengthMm);
    FixedWheelDriveControllerBoostMode(
        Motor &leftMotor, Motor &rightMotor,
        uint8_t wheelDistanceWidthMm, uint8_t wheelDistanceLengthMm);
    ~FixedWheelDriveControllerBoostMode(){};

    void loop() override;

protected:
    void setSpeedInternal(int16_t speed) override;
};

#endif