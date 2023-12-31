#ifndef ULTRASONIC_DISTANCE_SENSOR_HPP
#define ULTRASONIC_DISTANCE_SENSOR_HPP

#include "DistanceSensor.hpp"

#define SPEED_OF_SOUND_MM_PER_MSEC 340

struct UltrasonicSensorConfig
{
    uint8_t triggerPin, echoPin;
};

class UltrasonicDistanceSensor : public DistanceSensor
{
private:
    uint8_t trigger;
    uint8_t echo;
    uint16_t timeoutUs;

protected:
    uint16_t echoTimeToMm(unsigned long echoTimeUs);

public:
    UltrasonicDistanceSensor(uint8_t triggerPin, uint8_t echoPin, uint16_t maxDistance = 1000);
    UltrasonicDistanceSensor(UltrasonicSensorConfig config);
    ~UltrasonicDistanceSensor(){};

    uint16_t getDistanceToClosestMm() override;
    int8_t getAngleToClosest() override;
};

#endif