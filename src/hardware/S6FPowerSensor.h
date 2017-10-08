#pragma once

#include "../IADCReader.h"
#include "../IScalarSensor.h"

class S6FPowerSensor : public IScalarSensor<float> {
private:
    float Vref = 0.165f;            // Output voltage with no current: ~ 0,165V
    float adcMaxVoltage;            // Max voltage supported by ADC
    float adcRatio;
    int voltageDivider;

    IADCReader *_adcReader;
public:
    S6FPowerSensor(IADCReader *adcReader) {
        _adcReader = adcReader;

        this->voltageDivider = 1;   // Voltage divider in hardware 
        this->adcRatio = _adcReader->maxVoltage() / _adcReader->resolution();
        this->Vref = this->Vref * this->voltageDivider;
    }

    float readValue() {
        int rawAdcValue = _adcReader->analogRead();
        return (1.665 - ((3.3 / 4096.00) * (float) rawAdcValue)) / 0.110;
    }

};


