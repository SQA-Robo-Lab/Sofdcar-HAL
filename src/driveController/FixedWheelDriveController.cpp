#include "FixedWheelDriveController.hpp"

#define SIMPLE_MODE_FACTOR_OUT 1.5
#define SIMPLE_MODE_FACTOR_IN 0.75

FixedWheelDriveController::FixedWheelDriveController(
    Motor &frontLeftMotor, Motor &frontRightMotor,
    Motor &rearLeftMotor, Motor &rearRightMotor,
    uint8_t wheelDistanceWidthMm, uint8_t wheelDistanceLengthMm)
    : frontLeft(frontLeftMotor), frontRight(frontRightMotor),
      rearLeft(rearLeftMotor), rearRight(rearRightMotor),
      carWidthMm(wheelDistanceWidthMm), carLengthMm(wheelDistanceLengthMm)
{
}

FixedWheelDriveController::FixedWheelDriveController(
    Motor &leftMotor, Motor &rightMotor,
    uint8_t wheelDistanceWidthMm, uint8_t wheelDistanceLengthMm)
    : FixedWheelDriveController(leftMotor, rightMotor, leftMotor, rightMotor, wheelDistanceWidthMm, wheelDistanceLengthMm)
{
}

void FixedWheelDriveController::setSpeedInternal(int16_t speed)
{
    if (this->angle == 0)
    {
        this->rearLeft.setSpeed(speed);
        this->rearRight.setSpeed(speed);
        this->frontLeft.setSpeed(speed);
        this->frontRight.setSpeed(speed);
        return;
    }

    if (this->simpleMode)
    {
        if (this->angle > 15)
        {
            Serial.println("Simple turn right");
            this->rearLeft.setSpeed(1.5 * speed);
            this->rearRight.setSpeed(-1.5 * speed);
            this->frontLeft.setSpeed(1.5 * speed);
            this->frontRight.setSpeed(-1.5 * speed);
        }
        else if (this->angle < -15)
        {
            Serial.println("Simple turn left");
            this->rearLeft.setSpeed(-1.5 * speed);
            this->rearRight.setSpeed(1.5 * speed);
            this->frontLeft.setSpeed(-1.5 * speed);
            this->frontRight.setSpeed(1.5 * speed);
        }
        else if (this->angle > 5)
        {
            Serial.println("Simple right");
            this->rearLeft.setSpeed(SIMPLE_MODE_FACTOR_OUT * speed);
            this->rearRight.setSpeed(SIMPLE_MODE_FACTOR_IN * speed);
            this->frontLeft.setSpeed(SIMPLE_MODE_FACTOR_OUT * speed);
            this->frontRight.setSpeed(SIMPLE_MODE_FACTOR_IN * speed);
        }
        else if (this->angle < -5)
        {
            Serial.println("Simple left");
            this->rearLeft.setSpeed(SIMPLE_MODE_FACTOR_IN * speed);
            this->rearRight.setSpeed(SIMPLE_MODE_FACTOR_OUT * speed);
            this->frontLeft.setSpeed(SIMPLE_MODE_FACTOR_IN * speed);
            this->frontRight.setSpeed(SIMPLE_MODE_FACTOR_OUT * speed);
        }
        else
        {
            Serial.println("Simple straight");
            this->rearLeft.setSpeed(speed);
            this->rearRight.setSpeed(speed);
            this->frontLeft.setSpeed(speed);
            this->frontRight.setSpeed(speed);
        }
        return;
    }

    float innerRadius = tan((90.0 - abs(this->angle)) * PI / 180.0) * this->carLengthMm;
    float middleRadius = innerRadius + this->carWidthMm / 2;
    float outerWheelSpeedFactor = (innerRadius + this->carWidthMm) / middleRadius;
    float innerWheelSpeedFactor = innerRadius / middleRadius;
    Serial.print("Factors: Outer: ");
    Serial.print(outerWheelSpeedFactor);
    Serial.print("; Inner: ");
    Serial.println(innerWheelSpeedFactor);
    if (this->angle > 0)
    {
        this->rearLeft.setSpeed(speed * outerWheelSpeedFactor);
        this->rearRight.setSpeed(speed * innerWheelSpeedFactor);
        this->frontLeft.setSpeed(speed * outerWheelSpeedFactor);
        this->frontRight.setSpeed(speed * innerWheelSpeedFactor);
    }
    else
    {
        this->rearLeft.setSpeed(speed * innerWheelSpeedFactor);
        this->rearRight.setSpeed(speed * outerWheelSpeedFactor);
        this->frontLeft.setSpeed(speed * innerWheelSpeedFactor);
        this->frontRight.setSpeed(speed * outerWheelSpeedFactor);
    }
}

void FixedWheelDriveController::setAngleInternal(int8_t angle)
{
    // todo
    this->setSpeedInternal(this->getSpeed());
}

void FixedWheelDriveController::loop()
{
    this->frontLeft.loop();
    this->frontRight.loop();
    this->rearLeft.loop();
    this->rearRight.loop();
}

void FixedWheelDriveController::setSimpleMode(bool on)
{
    this->simpleMode = on;
}