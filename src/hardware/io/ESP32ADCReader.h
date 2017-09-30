#pragma once

#include "../../IADCReader.h"
//#include <driver/adc.h>

//#include "mgos_adc.h"

#define NUM_SAMPLES 10
#define PIN 7

/**
 * ADC reader implementation for ESP32
 */
class ESP32ADCReader : public IADCReader {
public:
    ESP32ADCReader() {
        //adc1_config_width(ADC_WIDTH_12Bit);
        //adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_11db);
   //     mgos_adc_enable(PIN);
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
            //rawAdcValue += adc1_get_voltage(ADC1_CHANNEL_6);
           // rawAdcValue += mgos_adc_read(PIN);

            // wait 2 milliseconds before the next loop
            // for the analog-to-digital converter to settle
            // after the last reading:

            mgos_usleep(2000);
        }

        // Calculate average reading
        rawAdcValue = rawAdcValue / NUM_SAMPLES;
        return rawAdcValue;
    }
};
