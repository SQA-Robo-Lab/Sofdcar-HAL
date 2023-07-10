#include "FixedWheelDriveControllerBoostMode.hpp"

#define BOOST_TIME_MS 200

FixedWheelDriveControllerBoostMode::FixedWheelDriveControllerBoostMode(
    Motor &frontLeftMotor, Motor &frontRightMotor,
    Motor &rearLeftMotor, Motor &rearRightMotor,
    uint8_t wheelDistanceWidthMm, uint8_t wheelDistanceLengthMm)
    : FixedWheelDriveController(frontLeftMotor, frontRightMotor, rearLeftMotor, rearRightMotor, wheelDistanceWidthMm, wheelDistanceLengthMm)
{
    this->setSimpleMode(true);
}

FixedWheelDriveControllerBoostMode::FixedWheelDriveControllerBoostMode(
    Motor &leftMotor, Motor &rightMotor,
    uint8_t wheelDistanceWidthMm, uint8_t wheelDistanceLengthMm)
    : FixedWheelDriveControllerBoostMode(leftMotor, rightMotor, leftMotor, rightMotor, wheelDistanceWidthMm, wheelDistanceLengthMm)
{
}

void FixedWheelDriveControllerBoostMode::setSpeedInternal(int16_t speed)
{
    if ((this->getAngle() < 0 && this->getAngle() > -0) || speed == 0 || (speed == this->lastSpeed && this->getAngle() == this->lastAngle))
    {
        FixedWheelDriveController::setSpeedInternal(speed);
    }
    else
    {
        int16_t setTo = 2 * speed;
        FixedWheelDriveController::setSpeedInternal(setTo);
        Serial.print("Settting speed to ");
        Serial.println(setTo);
        this->requiredSpeed = speed;
        this->lastUpdate = millis();
    }
    this->lastSpeed = speed;
    this->lastAngle = this->getAngle();
}

void FixedWheelDriveControllerBoostMode::loop()
{
    unsigned long now = millis();
    if (this->requiredSpeed != 0 && now - this->lastUpdate > BOOST_TIME_MS)
    {
        Serial.print("Resetting speed to ");
        Serial.println(this->requiredSpeed);
        FixedWheelDriveController::setSpeedInternal(this->requiredSpeed);
        this->requiredSpeed = 0;
        this->lastUpdate = now;
    }
    FixedWheelDriveController::loop();
}