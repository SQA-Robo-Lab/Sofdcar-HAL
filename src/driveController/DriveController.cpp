#define DRIVE_CONTROLLER_DEBUG 1

#include "DriveController.hpp"

DriveController::DriveController()
{
}

void DriveController::drive(int8_t angle, int16_t speed)
{
    this->setAngle(angle);
    this->setSpeed(speed);
}

void DriveController::pause()
{
    this->setSpeedInternal(0);
    this->state = DRIVE_CONTROLLER_STATE_PAUSED;
}
void DriveController::resume()
{
    this->setSpeedInternal(this->speed);
    this->state = this->speed == 0 ? DRIVE_CONTROLLER_STATE_STOPPED : DRIVE_CONTROLLER_STATE_DRIVING;
}

void DriveController::setSpeed(int16_t speed)
{
#ifdef DRIVE_CONTROLLER_DEBUG
    Serial.print("Setting speed: ");
    Serial.println(speed);
#endif
    this->speed = speed;
    this->setSpeedInternal(speed);
    this->state = this->speed == 0 ? DRIVE_CONTROLLER_STATE_STOPPED : DRIVE_CONTROLLER_STATE_DRIVING;
}
void DriveController::setAngle(int8_t angle)
{
#ifdef DRIVE_CONTROLLER_DEBUG
    Serial.print("Setting angle: ");
    Serial.println(angle);
#endif
    this->angle = angle;
    this->setAngleInternal(angle);
}

int16_t DriveController::getSpeed()
{
#ifdef DRIVE_CONTROLLER_DEBUG
    Serial.print("Getting speed: ");
    Serial.print(this->speed);
    Serial.print(", at state ");
    Serial.println(this->state);
#endif
    return this->state == DRIVE_CONTROLLER_STATE_DRIVING ? this->speed : 0;
}
int16_t DriveController::getNonPausedSpeed()
{
    return this->speed;
}
int8_t DriveController::getAngle()
{
    return this->angle;
}

DriveControllerState DriveController::getState()
{
    return this->state;
}