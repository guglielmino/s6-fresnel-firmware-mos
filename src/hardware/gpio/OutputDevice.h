//
// Created by Fabrizio Guglielmino on 23/09/17.
//

#pragma once

#include "mgos_gpio.h"

class OutputDevice {
public:
    typedef enum {
        ON,
        OFF
    } SwitchMode;

private:
    int _pin;


public:
    OutputDevice(int pin) : _pin(pin) {
        mgos_gpio_set_mode(pin, MGOS_GPIO_MODE_OUTPUT);
    }

    void turn(SwitchMode mode) {
        mgos_gpio_write(_pin, mode == ON);
    }

    void toggle() {
        mgos_gpio_toggle(_pin);
    }

};


