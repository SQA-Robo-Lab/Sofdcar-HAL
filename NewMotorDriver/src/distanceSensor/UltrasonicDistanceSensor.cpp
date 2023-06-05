#include "UltrasonicDistanceSensor.hpp"

UltrasonicDistanceSensor::UltrasonicDistanceSensor(uint8_t triggerPin, uint8_t echoPin, uint16_t maxDistance)
    : trigger(triggerPin), echo(echoPin)
{
    this->timeoutUs = maxDistance * 1000 / SPEED_OF_SOUND_MM_PER_MSEC;
}

UltrasonicDistanceSensor::UltrasonicDistanceSensor(uint8_t triggerEchoPin, uint16_t maxDistance) : trigger(triggerEchoPin), echo(triggerEchoPin)
{
}

UltrasonicDistanceSensor::~UltrasonicDistanceSensor()
{
}

uint16_t UltrasonicDistanceSensor::echoTimeToMm(uint16_t echoTimeUs)
{
    if (echoTimeUs == 0)
    {
        return DISTANCE_SENSOR_NO_OBJECT;
    }
    else
    {
        return echoTimeUs * SPEED_OF_SOUND_MM_PER_MSEC / (1000 * 2);
    }
}

uint16_t UltrasonicDistanceSensor::getDistanceToClosestMm()
{
    unsigned long pulseUs;
    if (this->trigger == this->echo)
    {
        pinMode(this->trigger, OUTPUT);
        pinMode(this->echo, INPUT);
        digitalWrite(this->trigger, LOW);
        delayMicroseconds(5);
        digitalWrite(this->trigger, HIGH);
        delayMicroseconds(10);
        digitalWrite(this->trigger, LOW);
    }
    else
    {
        pinMode(this->trigger, OUTPUT);
        digitalWrite(this->trigger, LOW);
        delayMicroseconds(5);
        digitalWrite(this->trigger, HIGH);
        delayMicroseconds(10);
        digitalWrite(this->trigger, LOW);
        pinMode(this->echo, INPUT);
    }
    pulseUs = pulseIn(this->echo, HIGH, this->timeoutUs) - 10;

    return this->echoTimeToMm(pulseUs);
}

int8_t UltrasonicDistanceSensor::getAngleToClosest()
{
    return 0;
}