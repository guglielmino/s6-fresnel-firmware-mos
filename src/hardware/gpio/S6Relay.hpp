//
// Created by Fabrizio Guglielmino on 16/01/18.
//

#pragma once

#include "mgos_gpio.h"
#include "../../interfaces/IOutputDevice.h"

class S6Relay : public IOutputDevice {

private:
    int _setpin, _resetpin;
    bool on;


public:
    S6Relay(int setpin, int resetpin) : _setpin(setpin), _resetpin(resetpin) {
        mgos_gpio_set_mode(setpin, MGOS_GPIO_MODE_OUTPUT);
        mgos_gpio_set_mode(resetpin, MGOS_GPIO_MODE_OUTPUT);
        on = false;
    }

    void turn(SwitchMode mode) {
        switch (mode){
            case SwitchMode::ON:
                pulse(_setpin);
                on = true;
                break;
            case SwitchMode::OFF:
                pulse(_resetpin);
                on = false;
                break;
        }
    }

    void toggle() {
        pulse(on ? _setpin : _resetpin);
        on = !on;
    }

private:

    void pulse(int pin) {
        mgos_gpio_write(pin, true);
        mgos_usleep(250000);
        mgos_gpio_write(pin, false);
    }

};