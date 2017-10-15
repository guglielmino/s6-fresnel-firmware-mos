#pragma once

#include "../interfaces/IScalarSensor.h"
#include "../interfaces/IADCReader.h"

#if CS_PLATFORM == CS_P_ESP8266
#include "../hardware/SONOFFPowerSensor.h"
#include "../hardware/SONOFFDailyKwh.h"
#elif CS_PLATFORM == CS_P_ESP32
#define ADC_PIN 34
#include "../hardware/io/ESP32ADCReader.h"
#include "../hardware/S6FPowerSensor.h"
#include "../hardware/S6DailyKwh.h"
#endif

IScalarSensor<float> *getPowerSensor() {
#if CS_PLATFORM == CS_P_ESP8266
    LOG(LL_DEBUG, ("SONOFF PowerSensor"));
    return new SONOFFPowerSensor();
#elif CS_PLATFORM == CS_P_ESP32
    LOG(LL_DEBUG, ("S6 Fresnel PowerSensor"));
    IADCReader *adcReader = new ESP32ADCReader(ADC_PIN);
    return new S6FPowerSensor(adcReader);
#endif
}

IScalarSensor<unsigned long> *getDailyKwhSensor() {
#if CS_PLATFORM == CS_P_ESP8266
    return new SONOFFDailyKwh();
#elif CS_PLATFORM == CS_P_ESP32
    return new S6DailyKwh();
#endif
}