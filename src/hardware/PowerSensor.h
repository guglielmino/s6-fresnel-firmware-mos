#pragma once

#include "../IADCReader.h"
#include "../IScalarSensor.h"

class PowerSensor : public IScalarSensor<float> {
    private:    
      float Vref = 0.165f;            // Output voltage with no current: ~ 0,165V
      float adcMaxVoltage;            // Max voltage supported by ADC
      float adcRatio;
      int voltageDivider; 

      IADCReader *_adcReader;
    public:
      PowerSensor(IADCReader *adcReader) {
        _adcReader = adcReader;

        this->voltageDivider = 1;   // Voltage divider in hardware 
        this->adcRatio = _adcReader->maxVoltage() / _adcReader->resolution();
        this->Vref = this->Vref * this->voltageDivider;                       
      }

      /*
       valore letto dall'ADC = 166
      166 * 0,0009765 V/divisione = 0,162099 V
      0,162099 * VoltageDivider = 0,324198V
      0,324198 - 0,33 = 0,0058A (corrisponde a 0A)
       */
      float readValue() {
        int rawAdcValue = _adcReader->analogRead();
       
        return ((rawAdcValue * this->adcRatio) * this->voltageDivider) - this->Vref;
      }
};


