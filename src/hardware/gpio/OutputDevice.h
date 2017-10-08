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
    typedef void (*switch_feedback_t)(SwitchMode newMode);

private:
    int _pin;
    OutputDevice::switch_feedback_t _switch_cb;

    void notifyCallback() {
        if (this->_switch_cb != nullptr) {
            // NOTA: LEGGERE DAL PIN SETTATO IN OUT MNDA IN CRASH ...
            //this->_switch_cb((mgos_gpio_read(_pin) ? SwitchMode::ON : SwitchMode::OFF));
        }
    }

public:
    OutputDevice(int pin) : _pin(pin) {
        mgos_gpio_set_mode(pin, MGOS_GPIO_MODE_OUTPUT);
    }

    void setSwitchCb(OutputDevice::switch_feedback_t cb) {
        _switch_cb = cb;
    }

    void turn(SwitchMode mode) {
        mgos_gpio_write(_pin, mode == ON);
        notifyCallback();
    }

    void toggle() {
        mgos_gpio_toggle(_pin);
        notifyCallback();
    }

};


