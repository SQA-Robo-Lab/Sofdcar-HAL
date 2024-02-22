#ifndef SERIAL_DEBUG_HPP
#define SERIAL_DEBUG_HPP

#include <Sofdcar-HAL.hpp>

void SerialDebug_loop(DriveController &dc, BrightnessSensorAnalog &sensor, LinearSensorEdgeDetector &ld);
extern bool isManualMode;

#endif