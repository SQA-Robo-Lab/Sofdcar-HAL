#ifndef SOFDCAR_HAL_LINE_FOLLOWER_HPP
#define SOFDCAR_HAL_LINE_FOLLOWER_HPP

#include "../lineDetector/LineDetector.hpp"
#include "../driveController/DriveController.hpp"
#include "../distanceSensor/DistanceSensor.hpp"
#include "../lineDetector/LinearSensorEdgeDetector.hpp"

enum LineFollowMode
{
    LINE_FOLLOW_MODE_NONE = 0,
    LINE_FOLLOW_MODE_ONLY_LINE = 1,
    LINE_FOLLOW_MODE_ONLY_DISTANCE = 2,
    LINE_FOLLOW_MODE_FULL = 255,
    LINE_FOLLOW_MODE_SPECIAL = 51,
};

class LineFollower
{
private:
    LineDetector *line;
    DriveController *drive;
    DistanceSensor *distance;
    uint8_t followingEdgeNum = 0;
    LineFollowMode mode = LINE_FOLLOW_MODE_FULL;
    DetectedLine lastLine;

public:
    LineFollower(LineDetector *lineDetector, DriveController *driveController, DistanceSensor *frontDistance);
    ~LineFollower() {};

    void loop();
    void setLineToFollow(uint8_t index);
    uint8_t getLineToFollow();
    void setMode(LineFollowMode mode);
    LineFollowMode getMode();
};

#endif