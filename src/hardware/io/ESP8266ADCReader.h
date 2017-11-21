//
// Created by Fabrizio Guglielmino on 01/10/17.
//

#pragma one

class ESP8266ADCReader : public IADCReader {
private:
    int _pin;
public:
    ESP8266ADCReader(int pin) : _pin(pin) {
        mgos_adc_enable(_pin);
    }

    int resolution() {
        return 1024;
    }

    float maxVoltage() {
        return 3.3;
    }

    int analogRead() {
        return mgos_adc_read(_pin);
    }
};
