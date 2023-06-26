#include <Arduino.h>
#include <Sofdcar-HAL.hpp>

uint8_t numElements = 10;
uint8_t counts[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned long times[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

uint8_t readSpeed = 0;

uint16_t currStep = 0;
unsigned long nextStepAt = 0;

MotorDcHBridge frontLeft(MOTOR_SPEED_UNIT_RPM, 2, 4, 3);
MotorDcHBridge frontRight(MOTOR_SPEED_UNIT_RPM, 7, 5, 6);
MotorDcHBridge rearLeft(MOTOR_SPEED_UNIT_RPM, 8, 10, 9);
MotorDcHBridge rearRight(MOTOR_SPEED_UNIT_RPM, 13, 11, 12);

void onFree()
{
    counts[0]++;
}

void setup()
{

    Serial.begin(115200);
    Serial.println("Starting");

    pinMode(2, INPUT);

    unsigned long start = micros();
    for (uint8_t i = 0; i < numElements; i++)
    {
        times[i] = start;
    }
    Serial.println("End setup");
    attachInterrupt(digitalPinToInterrupt(18), onFree, FALLING);
}

void loop()
{
    while (Serial.available())
    {
        char read = Serial.read();
        if (read == '\n')
        {
            Serial.print("Setting to ");
            Serial.println(readSpeed);
            frontRight.setSpeed(readSpeed);
            readSpeed = 0;
        }
        else if (read >= '0' && read <= '9')
        {
            readSpeed = readSpeed * 10 + read - '0';
        }
        else if (read == 't')
        {
            Serial.println("Starting automatic test");
            currStep = 255;
            frontRight.setSpeed(currStep);
            delay(1000);
        }
    }

    float cpus = 0;
    unsigned long now = micros();
    unsigned long last = times[numElements - 1];
    for (uint8_t i = numElements - 1; i >= 0 && i < 255; i--)
    {
        cpus += counts[i];
        if (i > 0)
        {
            counts[i] = counts[i - 1];
            times[i] = times[i - 1];
        }
        else
        {
            counts[i] = 0;
            times[i] = now;
        }
    }
    cpus = cpus / (now - last);

    if (currStep > 0 && currStep < 256)
    {
        if ((now - nextStepAt) >= 500000)
        {
            frontRight.setSpeed(currStep);
            nextStepAt = now;
            Serial.print("At speed ");
            Serial.print(currStep);
            Serial.print(", ");
            Serial.print(cpus * 60000000.0 / 20);
            Serial.println(" rpm");
            currStep--;
        }
    }
    else
    {
        currStep = 0;
        Serial.print("Pulses per ms: ");
        Serial.print(cpus);
        Serial.print(", Pulses per s: ");
        Serial.print(cpus * 1000000.0);
        Serial.print(", Rpm: ");
        Serial.println(cpus * 60000000.0 / 20);
    }

    delay(50);
}
