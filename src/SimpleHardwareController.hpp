#ifndef SOFDCAR_HAL_SIMPLE_HARDWARE_CONTROLLER_HPP
#define SOFDCAR_HAL_SIMPLE_HARDWARE_CONTROLLER_HPP

#include <Arduino.h>
#include <Sofdcar-HAL.hpp>

// template <uint8_t numSensors>
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
    UltrasonicSensorConfig frontUltrasonic, rearUltrasonic;
};

struct TurnSteeringCarConfig
{
    MotorPinConfig rearLeft, rearRight;
    ServoMotorConfig steering;
    uint8_t carWidth, carLength;
    UltrasonicSensorConfig frontUltrasonic, rearUltrasonic;
};

class SimpleHardwareController
{
private:
    DriveController *driveController;
    LinearSensorEdgeDetector *lineDetector; // todo: change back to abstract as soon as new interface for line detector
    DistanceSensor **distanceSensors;
    uint8_t numDistanceSensors;
    BrightnessSensor *brightness;
    Motor **motors;
    uint8_t numMotors;
    SteerableAxle **axles;
    uint8_t numAxles;
    LineFollower *lineFollower;
    bool initialized;

public:
    SimpleHardwareController();
    ~SimpleHardwareController();

    // template <uint8_t numSensors>
    uint8_t initializeCar(FixedSteeringCarConfig fixedSteeringConfig, LineSensorConfig lineSensorConfig);
    // template <uint8_t numSensors>
    uint8_t initializeCar(TurnSteeringCarConfig turnSteeringConfig, LineSensorConfig lineSensorConfig);

    DriveController *getDriveController();
    LineDetector *getLineDetector();
    DistanceSensor *getDistanceSensor(uint8_t sensorNum = 0);
    LineFollower *getLineFollower();

    void loop();

protected:
    // template <uint8_t numSensors>
    uint8_t initLineDetector(LineSensorConfig lineSensorConfig);
};

#endif