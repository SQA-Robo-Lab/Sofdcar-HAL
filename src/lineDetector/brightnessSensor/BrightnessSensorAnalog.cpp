#include "BrightnessSensorAnalog.hpp"

// #define BRIGHTNESS_SENSOR_DEBUG

float floatMap(float val, float inMin, float inMax, float outMin, float outMax)
{
    return (val - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

BrightnessSensorAnalog::BrightnessSensorAnalog(uint8_t *pinNumbers, uint8_t numberOfPins, BrightnessThresholds *briThretholds = nullptr)
    : pinsLen(numberOfPins), thresholds(briThretholds)
{
    this->pins = new uint8_t[numberOfPins];
    memcpy(this->pins, pinNumbers, numberOfPins);
}
BrightnessSensorAnalog::~BrightnessSensorAnalog()
{
    delete[] this->pins;
}

uint8_t BrightnessSensorAnalog::getRawValues(uint16_t *array, uint8_t maxLen)
{
    uint16_t i = 0;
#ifdef BRIGHTNESS_SENSOR_DEBUG
    Serial.print("Raw brightness values: ");
#endif
    for (; i < this->pinsLen && i < maxLen; i++)
    {
        uint16_t val = analogRead(this->pins[i]);
#ifdef BRIGHTNESS_SENSOR_DEBUG
        Serial.print(val);
        Serial.print("; ");
#endif
        array[i] = val;
    }
#ifdef BRIGHTNESS_SENSOR_DEBUG
    Serial.println();
#endif
    return i;
}

uint8_t BrightnessSensorAnalog::getValues(float *array, uint8_t maxLen)
{
    uint16_t i = 0;
#ifdef BRIGHTNESS_SENSOR_DEBUG
    Serial.print("Raw brightness values: ");
#endif
    for (; i < this->pinsLen && i < maxLen; i++)
    {
        uint16_t val = analogRead(this->pins[i]);
#ifdef BRIGHTNESS_SENSOR_DEBUG
        Serial.print(val);
        Serial.print("; ");
#endif
        float resVal = val;
        if (this->mappingFunctions != nullptr)
        {
            resVal = this->mappingFunctions[i](val);
        }
        else if (this->thresholds != nullptr)
        {
            resVal = floatMap(val, this->thresholds[i].blackThreshold, this->thresholds[i].whiteThreshold, 0, 1);
        }
        else
        {
            resVal = floatMap(val, 0, 1024, 0, 1);
        }
        array[i] = resVal;
    }
#ifdef BRIGHTNESS_SENSOR_DEBUG
    Serial.println();
#endif
    return i;
}

uint8_t BrightnessSensorAnalog::numberOfSensors()
{
    return this->pinsLen;
}

void BrightnessSensorAnalog::setTranslationFunctions(MappingFunction *functions)
{
    this->mappingFunctions = functions;
    this->thresholds = nullptr;
}

void BrightnessSensorAnalog::setThresholds(BrightnessThresholds *briThretholds)
{
    this->thresholds = briThretholds;
    this->mappingFunctions = nullptr;
}

uint8_t BrightnessSensorAnalog::calibrateFor(uint16_t **valuesArray, uint8_t numCaptures)
{
    unsigned long start;
    for (uint8_t i = 0; i < numCaptures; i++)
    {
        while (Serial.available())
        {
            Serial.read();
        }
        Serial.println("Then press ENTER");
        start = millis();
        while (!Serial.available() && Serial.read() != '\n')
        {
            if (millis() - start > 30000)
            {
                Serial.println("Timeout. Canceling calibration");
                return 1;
            }
        }
        Serial.println("----Capturing----");
        this->getRawValues(valuesArray[i], this->pinsLen);

        if (i < numCaptures)
        {
            Serial.println("Now place it slightly diffrently (e.g. on a bump so thet the sensor distance is different)");
            Serial.println("But still ensuring, all sensors see the same color");
        }
    }

    return 0;
}

void BrightnessSensorAnalog::calibrateSensors()
{
    this->calibrateSensors(this->thresholds);
}

void BrightnessSensorAnalog::calibrateSensors(BrightnessThresholds *briThretholds)
{
    if (briThretholds == nullptr)
    {
        return;
    }
    uint8_t numCaptures = 3;
    uint16_t *valuesLine[numCaptures];
    uint16_t *valuesNonLine[numCaptures];

    for (uint8_t i = 0; i < numCaptures; i++)
    {
        valuesLine[i] = new uint16_t[this->pinsLen];
        valuesNonLine[i] = new uint16_t[this->pinsLen];
    }

    Serial.println("Brigthness calibration started.");
    Serial.println("Place the car on the white (non line) part of the track.");
    Serial.println("Ensure all sensors only see white");

    if (calibrateFor(valuesNonLine, numCaptures) != 0)
    {
        return;
    }
    Serial.println("Now place the car on the black (line) part of the track.");
    Serial.println("Ensure all sensors only see black");
    if (calibrateFor(valuesLine, numCaptures) != 0)
    {
        return;
    }

    uint8_t blackIsSmaller[this->pinsLen];
    memset(blackIsSmaller, 1, this->pinsLen);
    for (uint8_t j = 0; j < this->pinsLen; j++)
    {
        for (uint8_t i = 0; i < numCaptures; i++)
        {
            if (valuesLine[i][j] > valuesNonLine[i][j])
            {
                blackIsSmaller[j] = 0;
            }
            else if (valuesLine[i][j] < valuesNonLine[i][j] && blackIsSmaller[j] == 0)
            {
                Serial.print("For sensor number ");
                Serial.print(j);
                Serial.print(" line/non-line brightness could not be determined! On line sometimes brighter, sometimes dimmer");
                Serial.println("Cancelled calibration");
                return;
            }
            else
            {
                Serial.print("For sensor number ");
                Serial.print(j);
                Serial.print(" line/non-line brightness could not be determined! Same value for on/off line");
                Serial.println("Cancelled calibration");
            }
        }
    }

    Serial.println("----Finished capturing----");
    Serial.println("Calibrated values:");
    for (uint8_t j = 0; j < this->pinsLen; j++)
    {
        for (uint8_t i = 1; i < numCaptures; i++)
        {
            if (blackIsSmaller[j])
            {
                // The line returns smaller values => use the BIGGEST captured value for the line and the SMALLEST for non-line => smallest inbetween interval
                valuesLine[0][j] = max(valuesLine[0][j], valuesLine[i][j]);
                valuesNonLine[0][j] = min(valuesNonLine[0][j], valuesNonLine[i][j]);
            }
            else
            {
                // The line returns bigger values => use the SMALLEST captured value for the line and the BIGGEST for non-line => smallest inbetween interval
                valuesLine[0][j] = max(valuesLine[0][j], valuesLine[i][j]);
                valuesNonLine[0][j] = min(valuesNonLine[0][j], valuesNonLine[i][j]);
            }
        }
        briThretholds[j].blackThreshold = valuesLine[0][j];
        briThretholds[j].whiteThreshold = valuesNonLine[0][j];
        Serial.print("Sensor ");
        Serial.print(j);
        Serial.print(" : Line:");
        Serial.print(briThretholds[j].blackThreshold);
        Serial.print(" : Non-Line:");
        Serial.println(briThretholds[j].whiteThreshold);
    }

    for (uint8_t i = 0; i < numCaptures; i++)
    {
        delete valuesLine[i];
        delete valuesNonLine[i];
    }
}