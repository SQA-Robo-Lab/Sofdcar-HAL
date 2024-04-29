#ifndef METADATA_CONFIG_HPP
#define METADATA_CONFIG_HPP

#include "Arduino.h"

namespace CarHardwareType
{
    enum Type
    {
        TURN_STEERING,
        FIXED_WHEEL
    };
};

struct MetadataConfig
{
    uint8_t carId, apiRevision;
    CarHardwareType::Type hardwareType;
};

#endif