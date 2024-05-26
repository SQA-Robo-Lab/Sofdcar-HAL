#ifndef HARDWARE_CONFIG_HPP
#define HARDWARE_CONFIG_HPP

#include "Arduino.h"

struct LineSensorConfig
{
    uint8_t sensorDistanceMm;
    uint8_t numSensors;
    uint8_t *sensorPins;
    BrightnessThresholds *thresholds;
};

struct FixedSteeringCarConfig
{
    MotorPinConfig frontLeft, frontRight, rearLeft, rearRight;
    uint8_t carWidth, carLength;
    uint16_t wheelCircumference;
    UltrasonicSensorConfig frontUltrasonic, rearUltrasonic;
};

struct TurnSteeringCarConfig
{
    MotorPinConfig rearLeft, rearRight;
    ServoMotorConfig steering;
    uint8_t carWidth, carLength;
    uint16_t wheelCircumference;
    UltrasonicSensorConfig frontUltrasonic, rearUltrasonic;
    int16_t *rpmLut;
};

#endif