//
// Created by Fabrizio Guglielmino on 23/09/17.
//

#pragma once

#include "mgos_gpio.h"

class OutputDevice  {
    private:
        int _pin;

    public:
        typedef enum {
            ON,
            OFF
        } SwitchMode;

    OutputDevice(int pin) : _pin(pin){
        mgos_gpio_set_mode(pin, MGOS_GPIO_MODE_OUTPUT);
    }

    void turn(SwitchMode mode) {
        mgos_gpio_write(_pin, mode==ON);
    }

};


