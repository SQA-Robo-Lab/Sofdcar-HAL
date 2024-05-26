#ifndef SOFDCAR_HAL_SIMPLE_HARDWARE_CONTROLLER_HPP
#define SOFDCAR_HAL_SIMPLE_HARDWARE_CONTROLLER_HPP

#include <Arduino.h>
#include <Sofdcar-HAL.hpp>

class SimpleHardwareController
{
private:
    DriveController *driveController;
    LineDetector *lineDetector;
    DistanceSensor **distanceSensors;
    uint8_t numDistanceSensors;
    BrightnessSensor *brightness;
    Motor **motors;
    uint8_t numMotors;
    SteerableAxle **axles;
    uint8_t numAxles;
    LineFollower *lineFollower;
    MotorProfile *profile = nullptr;
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
    uint8_t getNumDistanceSensors();
    DistanceSensor *getDistanceSensor(uint8_t sensorNum = 0);
    LineFollower *getLineFollower();

    void loop();

protected:
    // template <uint8_t numSensors>
    uint8_t initLineDetector(LineSensorConfig lineSensorConfig);
};

#endif