#ifndef LINE_DETECTOR_HPP
#define LINE_DETECTOR_HPP

#include "Arduino.h"

#define LINE_POSITION_THRESHOLD_LOW 12
#define LINE_POSITION_THRESHOLD_HIGH 45
#define LINE_ANGLE_THRESHOLD_LOW 10
#define LINE_ANGLE_THRESHOLD_HIGH 20

#define LINE_DETECTOR_NO_LINE_FOUND -128

typedef enum
{
    LEFT_OF_LINE = 0,
    ON_LINE = 1,
    RIGHT_OF_LINE = 2,
    LINE_POSITION_UNKNOWN = 3
} LinePosition;

struct DetectedLine
{
    int8_t posMm, angle;
};

class LineDetector
{

public:
    LineDetector();
    ~LineDetector(){};

    LinePosition getLegacyPosition();
    LinePosition positionToLegacy(int8_t pos, int8_t angle);

    /**
     * @deprecated Usage not recommended. Not as flexible as getAllDetectedLines
     */
    virtual int8_t getLinePositionMm() = 0;
    /**
     * @deprecated Usage not recommended. Not as flexible as getAllDetectedLines
     */
    virtual int8_t getLineAngle() = 0;

    virtual uint8_t getAllDetectedLines(DetectedLine *result, uint8_t maxLenResult) = 0;
};

#endif