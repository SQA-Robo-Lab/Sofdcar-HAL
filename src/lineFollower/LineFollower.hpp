#ifndef SOFDCAR_HAL_LINE_FOLLOWER_HPP
#define SOFDCAR_HAL_LINE_FOLLOWER_HPP

#include "../lineDetector/LineDetector.hpp"
#include "../driveController/DriveController.hpp"
#include "../distanceSensor/DistanceSensor.hpp"

class LineFollower
{
private:
    LineDetector *line;
    DriveController *drive;
    DistanceSensor *distance;

public:
    LineFollower(LineDetector *lineDetector, DriveController *driveController, DistanceSensor *frontDistance);
    ~LineFollower(){};

    void loop();
    void setLineToFollow(uint8_t index);
};

#endif