#include "LineFollower.hpp"

LineFollower::LineFollower(LineDetector *lineDetector, DriveController *driveController, DistanceSensor *frontDistance)
    : line(lineDetector), drive(driveController), distance(frontDistance)
{
}

void LineFollower::loop()
{
    if ((this->mode & LINE_FOLLOW_MODE_ONLY_DISTANCE) == LINE_FOLLOW_MODE_ONLY_DISTANCE)
    {
        uint16_t d = this->distance->getDistanceToClosestMm();

        Serial.print("Current distance: ");
        Serial.println(d);

        if (d < 100 && this->drive->getSpeed() != 0)
        {
            this->drive->pause();
        }
        else if (d > 100 && this->drive->getSpeed() == 0)
        {
            this->drive->resume();
        }
    }

    if ((this->mode & LINE_FOLLOW_MODE_ONLY_LINE) == LINE_FOLLOW_MODE_ONLY_LINE)
    {
        DetectedLine pos[2];
        uint8_t numLines = this->line->getAllDetectedLines(pos, 2);
        if (numLines > 0)
        {
            this->drive->setAngle(pos[min(this->followingEdgeNum, numLines - 1)].posMm * 20 / 16);
        }
    }
}

void LineFollower::setLineToFollow(uint8_t index)
{
    this->followingEdgeNum = index;
}

void LineFollower::setMode(LineFollowMode mode)
{
    this->mode = mode;
}

LineFollowMode LineFollower::getMode()
{
    return this->mode;
}