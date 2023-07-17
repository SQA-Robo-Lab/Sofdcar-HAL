#ifndef SOFDCAR_HAL_LINE_FOLLOWER_HPP
#define SOFDCAR_HAL_LINE_FOLLOWER_HPP

#include "../lineDetector/LineDetector.hpp"
#include "../driveController/DriveController.hpp"
#include "../distanceSensor/DistanceSensor.hpp"
#include "../lineDetector/LinearSensorEdgeDetector.hpp"

class LineFollower
{
private:
    LinearSensorEdgeDetector *line; // todo: change back to abstract as soon as new interface for line detector
    DriveController *drive;
    DistanceSensor *distance;

public:
    LineFollower(LinearSensorEdgeDetector *lineDetector, DriveController *driveController, DistanceSensor *frontDistance);
    ~LineFollower(){};

    void loop();
    void setLineToFollow(uint8_t index);
};

#endif