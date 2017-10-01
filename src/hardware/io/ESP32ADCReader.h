#pragma once

#include "../../IADCReader.h"
//#include <driver/adc.h>

#include "mgos_adc.h"

#define NUM_SAMPLES 10


/**
 * ADC reader implementation for ESP32
 */
class ESP32ADCReader : public IADCReader {
private:
    int _pin;
public:
    ESP32ADCReader(int pin) : _pin(pin) {
        mgos_adc_enable(_pin);
    }

    int resolution() {
        return 4096;
    }

    float maxVoltage() {
        return 3.3;
    }

    int analogRead() {
        int rawAdcValue = 0;
        for (int i = 0; i < NUM_SAMPLES; i++) {
            rawAdcValue += mgos_adc_read(_pin);

            mgos_usleep(2000);
        }

        // Calculate average reading
        rawAdcValue = rawAdcValue / NUM_SAMPLES;
        return rawAdcValue;
    }
};
