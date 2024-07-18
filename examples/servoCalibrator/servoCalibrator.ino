#include "Servo.h"

Servo servo;

void setup()
{
    Serial.begin(115200);
    if (!servo.attached())
    {
        servo.attach(52);
    }
    Serial.println("Hallo");
}

uint8_t angle = 0;
bool isNeg = false;
void loop()
{
    while (Serial.available())
    {
        char c = Serial.read();
        if (c == '-')
        {
            isNeg = !isNeg;
        }
        else if (c == '\n')
        {
            Serial.println(isNeg ? -angle : angle);
            servo.write(isNeg ? -angle : angle);
            angle = 0;
        }
        else if (c >= '0' && c <= '9')
        {
            angle = (angle * 10) + (c - '0');
        }
    }
}