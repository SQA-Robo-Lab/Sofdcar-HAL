#ifndef DRIVE_CONTROLLER_HPP
#define DRIVE_CONTROLLER_HPP

#include "Arduino.h"

enum DriveControllerState
{
    DRIVE_CONTROLLER_STATE_STOPPED = 0,
    DRIVE_CONTROLLER_STATE_PAUSED = 1,
    DRIVE_CONTROLLER_STATE_DRIVING = 2,
};

class DriveController
{
protected:
    int16_t speed = 0;
    int8_t angle = 0;
    DriveControllerState state = DRIVE_CONTROLLER_STATE_STOPPED;

public:
    DriveController();
    ~DriveController(){};

    void drive(int8_t angle, int16_t speed);
    void pause();
    void resume();

    void setSpeed(int16_t speed);
    void setAngle(int8_t angle);
    int16_t getSpeed();
    int16_t getNonPausedSpeed();
    int8_t getAngle();
    DriveControllerState getState();

    virtual void loop() = 0;

protected:
    virtual void setSpeedInternal(int16_t speed) = 0;
    virtual void setAngleInternal(int8_t angle) = 0;
};

#endif