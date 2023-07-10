#include <Arduino.h>
#include <Sofdcar-HAL.hpp>

uint8_t numElements = 10;
uint8_t counts[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned long times[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

uint8_t readSpeed = 0;

int16_t currStep = 0;
int16_t pausedSpeed = 0;
unsigned long lastStepAt = 0;

MotorDcHBridge frontLeft(MOTOR_SPEED_UNIT_RPM, 2, 4, 3);
MotorDcHBridge frontRight(MOTOR_SPEED_UNIT_RPM, 7, 5, 6);
MotorDcHBridge rearLeft(MOTOR_SPEED_UNIT_RPM, 8, 10, 9);
MotorDcHBridge rearRight(MOTOR_SPEED_UNIT_RPM, 13, 11, 12);

int16_t rpmLut[256 * 2];

void onFree()
{
    counts[0]++;
}

void setSpeed(int32_t speed)
{
    frontLeft.setSpeed(speed);
    frontRight.setSpeed(speed);
    rearLeft.setSpeed(speed);
    rearRight.setSpeed(speed);
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
            if (currStep == 0)
            {
                Serial.print("Setting to ");
                Serial.println(readSpeed);
                setSpeed(readSpeed);
            }
            readSpeed = 0;
        }
        else if (read >= '0' && read <= '9')
        {
            readSpeed = readSpeed * 10 + read - '0';
        }
        else if (read == 't')
        {
            Serial.println("Starting automatic test");
            pausedSpeed = 0;
            currStep = 255;
            setSpeed(currStep);
            lastStepAt = micros() + 1000000;
        }
        else if (read == 'c')
        {
            currStep = 0;
            pausedSpeed = 0;
            setSpeed(currStep);
            Serial.println("Cancelled test");
        }
        else if (read == 'p')
        {
            pausedSpeed = currStep;
            currStep = 0;
            setSpeed(currStep);
            Serial.print("Paused automatic test at speed ");
            Serial.println(pausedSpeed);
        }
        else if (read == 'r')
        {
            Serial.print("Resuming automatic test at speed ");
            Serial.println(pausedSpeed);
            currStep = pausedSpeed;
            pausedSpeed = 0;
            setSpeed(currStep);
            lastStepAt = micros() + 1000000;
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

    if (currStep != 0)
    {
        unsigned long passed = now - lastStepAt;
        if (passed >= 500000 && passed < __LONG_MAX__)
        {
            int16_t lastStep = currStep;
            lastStepAt = now;
            if (currStep > 0)
            {
                currStep--;
                if (currStep == 0)
                {
                    lastStepAt = now + 1000000;
                    currStep = -256;
                }
            }
            else
            {
                currStep++;
                if (currStep == 0)
                {

                    Serial.println("Test result: ");
                    Serial.print("{");
                    for (uint16_t i = 0; i < 2 * 256; i++)
                    {
                        Serial.print(rpmLut[i]);
                        Serial.print(", ");
                    }
                    Serial.println("}");
                }
            }
            setSpeed(currStep);

            float rpm = cpus * 60000000.0 / 20;
            rpmLut[lastStep + 256] = floor(rpm * (lastStep < 0 ? -1 : 1));
            Serial.print("At speed ");
            Serial.print(lastStep);
            Serial.print(", ");
            Serial.print(rpmLut[lastStep + 256]);
            Serial.println(" rpm");
        }
    }
    else
    {
        /*currStep = 0;
        Serial.print("Pulses per ms: ");
        Serial.print(cpus);
        Serial.print(", Pulses per s: ");
        Serial.print(cpus * 1000000.0);
        Serial.print(", Rpm: ");
        Serial.println(cpus * 60000000.0 / 20);*/
    }

    delay(50);
}
