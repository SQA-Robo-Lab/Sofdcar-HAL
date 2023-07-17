#include "LineFollower.hpp"

LineFollower::LineFollower(LinearSensorEdgeDetector *lineDetector, DriveController *driveController, DistanceSensor *frontDistance)
    : line(lineDetector), drive(driveController), distance(frontDistance)
{
}

void LineFollower::loop()
{
    uint16_t d = this->distance->getDistanceToClosestMm();

    if (d < 100 && this->drive->getSpeed() != 0)
    {
        this->drive->pause();
    }
    else if (d > 100 && this->drive->getSpeed() == 0)
    {
        this->drive->resume();
    }

    int8_t lp = this->line->getLinePositionMm();
    this->drive->setAngle(lp * 20 / 16);
}

void LineFollower::setLineToFollow(uint8_t index)
{
    if (index == 0)
    {
        line->setFollowEdge(true);
    }
    else
    {
        line->setFollowEdge(false);
    }
}