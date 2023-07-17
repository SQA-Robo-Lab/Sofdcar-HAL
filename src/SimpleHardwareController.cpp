#include "SimpleHardwareController.hpp"

SimpleHardwareController::SimpleHardwareController()
{
    this->driveController = nullptr;
    this->lineDetector = nullptr;
    this->brightness = nullptr;
    this->lineFollower = nullptr;
    this->distanceSensors = nullptr;
    this->motors = nullptr;
    this->axles = nullptr;
    this->initialized = false;
}

SimpleHardwareController::~SimpleHardwareController()
{
    if (this->driveController != nullptr)
    {
        delete this->driveController;
    }
    if (this->lineDetector != nullptr)
    {
        delete this->lineDetector;
    }
    if (this->brightness != nullptr)
    {
        delete this->brightness;
    }
    if (this->lineFollower != nullptr)
    {
        delete this->lineFollower;
    }
    if (this->distanceSensors != nullptr)
    {
        for (uint8_t i = 0; i < this->numDistanceSensors; i++)
        {
            if (this->distanceSensors[i] != nullptr)
            {
                delete this->distanceSensors[i];
            }
        }
        delete this->distanceSensors;
    }
    if (this->motors != nullptr)
    {
        for (uint8_t i = 0; i < this->numMotors; i++)
        {
            if (this->motors[i] != nullptr)
            {
                delete this->motors[i];
            }
        }
        delete this->motors;
    }
    if (this->axles != nullptr)
    {
        for (uint8_t i = 0; i < this->numAxles; i++)
        {
            if (this->axles[i] != nullptr)
            {
                delete this->axles[i];
            }
        }
        delete this->axles;
    }
}

uint8_t SimpleHardwareController::initializeCar(FixedSteeringCarConfig fixedSteeringConfig, LineSensorConfig lineSensorConfig)
{
    if (this->initialized)
    {
        return 1;
    }
    this->initLineDetector(lineSensorConfig);
    this->numDistanceSensors = 2;
    this->distanceSensors = new DistanceSensor *[2];
    this->distanceSensors[0] = new UltrasonicDistanceSensor(fixedSteeringConfig.frontUltrasonic);
    this->distanceSensors[1] = new UltrasonicDistanceSensor(fixedSteeringConfig.rearUltrasonic);

    // todo: add motor profile adaptation
    this->numMotors = 4;
    this->motors = new Motor *[4];
    this->motors[0] = new MotorDcHBridge(fixedSteeringConfig.frontLeft);
    this->motors[1] = new MotorDcHBridge(fixedSteeringConfig.frontRight);
    this->motors[2] = new MotorDcHBridge(fixedSteeringConfig.rearLeft);
    this->motors[3] = new MotorDcHBridge(fixedSteeringConfig.rearRight);

    this->numAxles = 0;
    this->axles = nullptr;

    this->driveController = new FixedWheelDriveControllerBoostMode(
        *(this->motors[0]),
        *(this->motors[1]),
        *(this->motors[2]),
        *(this->motors[3]),
        fixedSteeringConfig.carWidth,
        fixedSteeringConfig.carLength);

    this->lineFollower = new LineFollower(this->lineDetector, this->driveController, this->distanceSensors[0]);

    this->initialized = true;
}

uint8_t SimpleHardwareController::initializeCar(TurnSteeringCarConfig turnSteeringConfig, LineSensorConfig lineSensorConfig)
{
    if (this->initialized)
    {
        return 1;
    }
    this->initLineDetector(lineSensorConfig);
    this->numDistanceSensors = 2;
    this->distanceSensors = new DistanceSensor *[2];
    this->distanceSensors[0] = new UltrasonicDistanceSensor(turnSteeringConfig.frontUltrasonic);
    this->distanceSensors[1] = new UltrasonicDistanceSensor(turnSteeringConfig.rearUltrasonic);

    this->numMotors = 2;
    this->motors = new Motor *[2];
    this->motors[0] = new MotorDcHBridge(turnSteeringConfig.rearLeft);
    this->motors[1] = new MotorDcHBridge(turnSteeringConfig.rearRight);

    this->numAxles = 1;
    this->axles = new SteerableAxle *[1];
    this->axles[0] = new ServoAxle(turnSteeringConfig.steering);

    this->driveController = new TurnSteeringDriveController(
        *(this->motors[0]),
        *(this->motors[1]),
        *(this->axles[0]),
        turnSteeringConfig.carWidth,
        turnSteeringConfig.carLength);

    this->lineFollower = new LineFollower(this->lineDetector, this->driveController, this->distanceSensors[0]);

    this->initialized = true;
}

uint8_t SimpleHardwareController::initLineDetector(LineSensorConfig lineSensorConfig)
{
    // todo: move config init to brightness sensor class
    // todo: copy threshods array
    this->brightness = new BrightnessSensorAnalog(lineSensorConfig.sensorPins, lineSensorConfig.numSensors, lineSensorConfig.thresholds);
    this->lineDetector = new LinearSensorEdgeDetector(*(this->brightness), lineSensorConfig.sensorDistanceMm, false);
}

DriveController *SimpleHardwareController::getDriveController()
{
    return this->driveController;
}

LineDetector *SimpleHardwareController::getLineDetector()
{
    return this->lineDetector;
}

DistanceSensor *SimpleHardwareController::getDistanceSensor(uint8_t sensorNum)
{
    return this->distanceSensors[sensorNum];
}

LineFollower *SimpleHardwareController::getLineFollower()
{
    return this->lineFollower;
}

void SimpleHardwareController::loop()
{
    if (this->driveController != nullptr)
    {
        this->driveController->loop();
    }
    if (this->lineFollower != nullptr)
    {
        this->lineFollower->loop();
    }
}