#include "LinearSensorLineDetector.hpp"

#define LINE_SENSOR_DEBUG

LinearSensorLineDetector::LinearSensorLineDetector(BrightnessSensor &sensor, uint8_t sensorDistanceMm)
    : sensorsLen(1), sensorDistance(sensorDistanceMm)
{
    this->sensors = new BrightnessSensor *[1];
    this->sensors[0] = &sensor;
    this->totalSensors = sensor.numberOfSensors();
}
LinearSensorLineDetector::LinearSensorLineDetector(BrightnessSensor **multipleSensors, uint8_t numberOfSensors, uint8_t sensorDistanceMm)
    : sensorsLen(numberOfSensors), sensorDistance(sensorDistanceMm)
{
    this->sensors = new BrightnessSensor *[numberOfSensors];
    memcpy(this->sensors, multipleSensors, numberOfSensors);
    this->totalSensors = 0;
    for (uint16_t i = 0; i < numberOfSensors; i++)
    {
        this->totalSensors += this->sensors[i]->numberOfSensors();
    }
}
LinearSensorLineDetector::~LinearSensorLineDetector()
{
    delete[] this->sensors;
}

int8_t LinearSensorLineDetector::getLinePositionMm()
{
    float values[this->totalSensors];
    uint32_t nextIndex = 0;
    for (uint16_t i = 0; i < this->sensorsLen; i++)
    {
        nextIndex += this->sensors[i]->getValues(values + nextIndex, this->totalSensors - nextIndex);
    }

#ifdef LINE_SENSOR_DEBUG
    Serial.print("Brigtness values: ");
#endif

    int8_t pos = LINE_DETECTOR_NO_LINE_FOUND;
    float maxVal = 0.2;
    float lowerValForMax = 0;
    uint8_t numLargerHalf = 0;

    float prevVal = 1 - values[0];
    if (prevVal > 0.5)
    {
        numLargerHalf++;
    }
#ifdef LINE_SENSOR_DEBUG
    Serial.print(prevVal);
    Serial.print("; ");
#endif

    for (nextIndex = 1; nextIndex < this->totalSensors; nextIndex++)
    {
        float currVal = 1 - values[nextIndex];

#ifdef LINE_SENSOR_DEBUG
        Serial.print(currVal);
        Serial.print("; ");
#endif

        if (currVal > 0.5)
        {
            numLargerHalf++;
        }

        if (currVal > maxVal || prevVal > maxVal || (currVal == maxVal && prevVal > lowerValForMax) || (prevVal == maxVal && currVal > lowerValForMax))
        {
            if (currVal > prevVal)
            {
                maxVal = currVal;
                lowerValForMax = prevVal;
            }
            else
            {
                maxVal = prevVal;
                lowerValForMax = currVal;
            }
            float largerVal = max(currVal, prevVal);
            float ratio = ((currVal / largerVal) - (prevVal / largerVal) + 1) / 2;
            pos = floor((ratio + (nextIndex - 1) - ((this->totalSensors - 1) / 2)) * this->sensorDistance);
        }

        prevVal = currVal;
    }
#ifdef LINE_SENSOR_DEBUG
    Serial.println();
#endif

    return pos;
}

int8_t LinearSensorLineDetector::getLineAngle()
{
    return 0;
}

uint8_t LinearSensorLineDetector::getAllDetectedLines(DetectedLine *result, uint8_t maxLenResult)
{
    if (maxLenResult > 0)
    {
        result[0].posMm = this->getLinePositionMm();
        result[0].angle = 0;
        return 1;
    }
    return 0;
}