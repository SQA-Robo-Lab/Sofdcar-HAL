#include "LinearSensorEdgeDetector.hpp"

#define LINE_SENSOR_DEBUG

#define NO_SENSOR_INDEX 255

LinearSensorEdgeDetector::LinearSensorEdgeDetector(BrightnessSensor &sensor, uint8_t sensorDistanceMm, bool followLeftEdge)
    : sensorsLen(1), sensorDistance(sensorDistanceMm), leftEdge(followLeftEdge)
{
    this->sensors = new BrightnessSensor *[1];
    this->sensors[0] = &sensor;
    this->totalSensors = sensor.numberOfSensors();
}
LinearSensorEdgeDetector::LinearSensorEdgeDetector(BrightnessSensor **multipleSensors, uint8_t numberOfSensors, uint8_t sensorDistanceMm, bool followLeftEdge)
    : sensorsLen(numberOfSensors), sensorDistance(sensorDistanceMm), leftEdge(followLeftEdge)
{
    this->sensors = new BrightnessSensor *[numberOfSensors];
    memcpy(this->sensors, multipleSensors, numberOfSensors);
    this->totalSensors = 0;
    for (uint8_t i = 0; i < numberOfSensors; i++)
    {
        this->totalSensors += this->sensors[i]->numberOfSensors();
    }
}
LinearSensorEdgeDetector::~LinearSensorEdgeDetector()
{
    delete[] this->sensors;
}

void LinearSensorEdgeDetector::setFollowEdge(bool followLeftEdge)
{
    this->leftEdge = followLeftEdge;
}

int8_t LinearSensorEdgeDetector::getLinePositionMm()
{
    float values[this->totalSensors];
    uint32_t nextIndex = 0;

    for (uint8_t i = 0; i < this->sensorsLen; i++)
    {
        nextIndex += this->sensors[i]->getValues(values + nextIndex, this->totalSensors - nextIndex);
    }

    return this->calcEdgePosFromValues(values, this->leftEdge);
}

int8_t LinearSensorEdgeDetector::getLineAngle()
{
    return 0;
}

int8_t LinearSensorEdgeDetector::calcEdgePosFromValues(float *values, bool leftEdge)
{
#ifdef LINE_SENSOR_DEBUG
    if (leftEdge)
    {
        Serial.print("--- Searching left edge ---");
    }
    else
    {
        Serial.print("--- Searching right edge ---");
    }
    Serial.print("Brigtness values: ");
#endif
    float fValues[this->totalSensors];
    for (uint8_t i = 0; i < this->totalSensors; i++)
    {
#ifdef LINE_SENSOR_DEBUG
        Serial.print("(");
        Serial.print(1 - values[i]);
        Serial.print("); ");
#endif
        fValues[leftEdge ? i : this->totalSensors - i - 1] = 1 - values[i];
    }
#ifdef LINE_SENSOR_DEBUG
    // Serial.println();
#endif

    uint8_t firstNonWhite = NO_SENSOR_INDEX;
    uint8_t firstBlackIndex = NO_SENSOR_INDEX;
    for (uint8_t i = 0; i < this->totalSensors; i++)
    {
        if (firstBlackIndex == NO_SENSOR_INDEX && fValues[i] >= 0.9)
        {
            firstBlackIndex = i;
        }
        if (firstNonWhite == NO_SENSOR_INDEX && fValues[i] > 0.1)
        {
            firstNonWhite = i;
        }
    }

#ifdef LINE_SENSOR_DEBUG
    Serial.print("; First non white: ");
    Serial.print(firstNonWhite);
    Serial.print("; First black: ");
    Serial.print(firstBlackIndex);
#endif

    float linePosRel = NAN;
    if (firstBlackIndex == firstNonWhite + 1)
    {
#ifdef LINE_SENSOR_DEBUG
        Serial.print("; A");
#endif
        // edge between first non white and first black
        linePosRel = firstBlackIndex - fValues[firstNonWhite];
    }
    else if (firstNonWhite != NO_SENSOR_INDEX && firstBlackIndex != NO_SENSOR_INDEX)
    {
        if (firstBlackIndex > 0)
        {
            if (firstNonWhite == firstBlackIndex)
            {
#ifdef LINE_SENSOR_DEBUG
                Serial.print("; B");
#endif
                // edge is (somewhere) between first black and previous
                linePosRel = firstBlackIndex;
            }
            else
            {
#ifdef LINE_SENSOR_DEBUG
                Serial.print("; C");
#endif
                // edge between first black and one previous
                linePosRel = firstBlackIndex - fValues[firstNonWhite];
            }
        }
        else
        {
#ifdef LINE_SENSOR_DEBUG
            Serial.print("; D");
#endif
            // black is at edge of sensor array; edge of line outside
            linePosRel = 0;
        }
    }
    else if (firstNonWhite != NO_SENSOR_INDEX && firstBlackIndex == NO_SENSOR_INDEX)
    {
        // no black found; edge either between first non white and next one or previous one (depending on value of next one)
        float firstNonWhiteValue = fValues[firstNonWhite];
        float nextValue = firstNonWhite < this->totalSensors - 1 ? fValues[firstNonWhite + 1] : 0;

        if (nextValue > 0)
        {
#ifdef LINE_SENSOR_DEBUG
            Serial.print("; E");
#endif
            // sensor after first non white is darker => line edge must be between that and the first non white

            /* Causes jumping, if better idea: refactor
            float scale = 1.0 / max(firstNonWhiteValue, nextValue);
            firstNonWhiteValue *= scale;
            nextValue *= scale;
            float posBetween = -firstNonWhiteValue + nextValue;
            if (posBetween < 0)
            {
                linePosRel = firstNonWhite + 1 + posBetween;
            }
            else
            {
                linePosRel = firstNonWhite + posBetween;
            }*/
            linePosRel = firstNonWhite + 1 - firstNonWhiteValue;
        }
        else
        {
            if (firstNonWhite > 0)
            {
#ifdef LINE_SENSOR_DEBUG
                Serial.print("; F");
#endif
                // sensor after first non white is white (as is the one previous) => line edge must be between previous and first non white
                linePosRel = firstNonWhite;
            }
            else
            {
#ifdef LINE_SENSOR_DEBUG
                Serial.print("; G");
#endif
                // sensor after first non white is white and first non white is the first sensor => line edge is propably outside
                linePosRel = 0;
            }
        }
    }

    if (linePosRel == linePosRel) // test if nan by comparing with itself
    {
        linePosRel -= (this->totalSensors - 1) * 0.5;
        linePosRel = leftEdge ? linePosRel : -linePosRel;
#ifdef LINE_SENSOR_DEBUG
        Serial.print("; Detected pos-: ");
        Serial.println(linePosRel);
#endif
        int8_t pos = round(linePosRel * this->sensorDistance);
        return pos;
    }
    else
    {
#ifdef LINE_SENSOR_DEBUG
        Serial.println("; Detected pos: nan!");
#endif
        return LINE_DETECTOR_NO_LINE_FOUND;
    }
}

uint8_t LinearSensorEdgeDetector::getAllDetectedLines(DetectedLine *result, uint8_t maxLenResult)
{
    float values[this->totalSensors];
    uint32_t nextIndex = 0;

    for (uint8_t i = 0; i < this->sensorsLen; i++)
    {
        nextIndex += this->sensors[i]->getValues(values + nextIndex, this->totalSensors - nextIndex);
    }

    int8_t linePosLeft = this->calcEdgePosFromValues(values, true);
    int8_t linePosRight = this->calcEdgePosFromValues(values, false);
    Serial.print(linePosLeft);
    Serial.print("; ");
    Serial.println(linePosRight);

    uint8_t filledSlots = 0;
    if (linePosLeft != LINE_DETECTOR_NO_LINE_FOUND)
    {
        if (maxLenResult > filledSlots)
        {
            result[filledSlots].posMm = linePosLeft;
            result[filledSlots].angle = 0;
            filledSlots++;
        }
    }
    if (linePosRight != LINE_DETECTOR_NO_LINE_FOUND)
    {
        if (maxLenResult > filledSlots)
        {
            result[filledSlots].posMm = linePosRight;
            result[filledSlots].angle = 0;
            filledSlots++;
        }
    }
    return filledSlots;
}