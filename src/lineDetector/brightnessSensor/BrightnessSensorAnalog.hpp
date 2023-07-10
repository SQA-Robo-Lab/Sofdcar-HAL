#ifndef BRIGHTNESS_SENSOR_ANALOG_HPP
#define BRIGHTNESS_SENSOR_ANALOG_HPP

#include "BrightnessSensor.hpp"

typedef float (*MappingFunction)(uint16_t oldVal);

struct BrightnessThresholds
{
    uint16_t blackThreshold;
    uint16_t whiteThreshold;
};

class BrightnessSensorAnalog : public BrightnessSensor
{
private:
    uint8_t pinsLen;
    uint8_t *pins;

    BrightnessThresholds *thresholds = nullptr;
    MappingFunction *mappingFunctions = nullptr;

    uint8_t calibrateFor(uint16_t **valuesArray, uint8_t numCaptures);
    uint8_t getRawValues(uint16_t *array, uint8_t maxLen);

public:
    BrightnessSensorAnalog(uint8_t *pinNumbers, uint8_t numberOfPins, BrightnessThresholds *briThretholds = nullptr);
    ~BrightnessSensorAnalog();

    uint8_t getValues(float *array, uint8_t maxLen) override;
    uint8_t numberOfSensors() override;

    /**
     * Number of functions must EXACTLY match number of pins
     * Setting this will unset the Thretholds
     */
    void setTranslationFunctions(MappingFunction *functions);

    /**
     * Number of threshold structs must EXACTLY match number of pins
     * Setting this will unset the translation functions
     */
    void setThresholds(BrightnessThresholds *briThretholds);

    /**
     * Run a (semi manual) calibration routine.
     * Needs access to the serial console
     */
    void calibrateSensors();
    void calibrateSensors(BrightnessThresholds *briThretholds);
};

#endif