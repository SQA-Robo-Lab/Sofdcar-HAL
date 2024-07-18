#include "LineFollower.hpp"

// #define LINE_FOLLOWER_DEBUG

LineFollower::LineFollower(LineDetector *lineDetector, DriveController *driveController, DistanceSensor *frontDistance)
    : line(lineDetector), drive(driveController), distance(frontDistance)
{
}

void LineFollower::loop()
{
    if ((this->mode & LINE_FOLLOW_MODE_ONLY_DISTANCE) == LINE_FOLLOW_MODE_ONLY_DISTANCE)
    {
        uint16_t d = this->distance->getDistanceToClosestMm();

#ifdef LINE_FOLLOWER_DEBUG
        Serial.print("Current distance: ");
        Serial.println(d);
#endif

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
            uint8_t lineNum = min(this->followingEdgeNum, numLines - 1);

#ifdef LINE_FOLLOWER_DEBUG
            Serial.print("Following edge ");
            Serial.print(lineNum);
            Serial.print(" at pos ");
            Serial.println(pos[lineNum].posMm);
#endif

            this->drive->setAngle(pos[lineNum].posMm * 35 / 16);
            lastLine = pos[lineNum];
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