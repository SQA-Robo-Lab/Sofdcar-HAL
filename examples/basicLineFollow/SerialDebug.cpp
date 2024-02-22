#include "serialDebug.hpp"
#include "Arduino.h"

void SerialDebug_loop(DriveController &dc, BrightnessSensorAnalog &sensor, LinearSensorEdgeDetector &ld)
{
    int16_t readAngle = INT16_MAX;
    while (Serial.available())
    {
        char c = Serial.read();
        switch (c)
        {
        case 'a':
            dc.setAngle(10);
            isManualMode = true;
            break;
        case 'b':
            dc.setAngle(-10);
            isManualMode = true;
            break;
        case 'c':
            dc.setAngle(30);
            isManualMode = true;
            break;
        case 'd':
            dc.setAngle(-30);
            isManualMode = true;
            break;
        case 'e':
            dc.setAngle(0);
            isManualMode = true;
            break;
        case '5':
            dc.setSpeed(0);
            break;
        case '6':
            dc.setSpeed(30);
            break;
        case '7':
            dc.setSpeed(40);
            break;
        case '8':
            dc.setSpeed(50);
            break;
        case '9':
            dc.setSpeed(60);
            break;
        case '4':
            dc.setSpeed(-30);
            break;
        case '3':
            dc.setSpeed(-40);
            break;
        case '2':
            dc.setSpeed(-50);
            break;
        case '1':
            dc.setSpeed(-60);
            break;
        case '!':
            sensor.calibrateSensors();
            break;
        case '-':
            isManualMode = false;
            break;
        case '<':
            ld.setFollowEdge(true);
            break;
        case '>':
            ld.setFollowEdge(false);
            break;
        }
    }
}