#ifndef LINEAR_SENSOR_LINE_DETECTOR_HPP
#define LINEAR_SENSOR_LINE_DETECTOR_HPP

#include "LineDetector.hpp"
#include "brightnessSensor/BrightnessSensor.hpp"

#define BRIGHTNESS_SENSOR_THRETHOLD 20

class LinearSensorLineDetector : public LineDetector
{
private:
    BrightnessSensor **sensors;
    uint8_t sensorsLen;
    uint8_t totalSensors;
    uint8_t sensorDistance;
    uint8_t whiteThrethold;
    uint8_t blackThrethold;

public:
    LinearSensorLineDetector(BrightnessSensor &sensor, uint8_t sensorDistanceMm, uint8_t definetlyWhiteThrethold, uint8_t definetlyBlackThrethold);
    LinearSensorLineDetector(BrightnessSensor **multipleSensors, uint8_t numberOfSensors, uint8_t sensorDistanceMm, uint8_t definetlyWhiteThrethold, uint8_t definetlyBlackThrethold);
    ~LinearSensorLineDetector();

    int8_t getLinePositionMm() override;
    int8_t getLineAngle() override;
};

#endif