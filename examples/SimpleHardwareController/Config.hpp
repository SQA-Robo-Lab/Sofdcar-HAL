#ifndef SIMPLE_HARDWARE_COINTROLLER_EXAMPLE_CONFIG_HPP
#define SIMPLE_HARDWARE_COINTROLLER_EXAMPLE_CONFIG_HPP

#include <SimpleHardwareController.hpp>

TurnSteeringCarConfig config = {
    {13, 12, 11},              // rearLeft
    {8, 9, 10},                // rearRight
    {52, 40, 39, 19, 66, 109}, // steering
    120,
    100,      // width, length
    204,      // wheel circumfrence
    {48, 49}, // frontDistance
    {48, 49}  // rearDistance
};

uint8_t brightnessPins[3] = {A0, A1, A2};
BrightnessThresholds thresholds[3] = {{41, 114},
                                      {61, 140},
                                      {31, 81}};

LineSensorConfig lineConfig = {
    16,             // sensor distance
    3,              // number of sesnor
    brightnessPins, // sensor pins
    thresholds      // sensor thretholds
};

#endif