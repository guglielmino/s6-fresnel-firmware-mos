//
// Created by Fabrizio Guglielmino on 08/10/17.
//

#pragma once

class InputDevice  {
private:
    int _pin;
public:

    typedef void (*event_callback_t)(bool newPinState);

    InputDevice(int pin, InputDevice::event_callback_t cb) : _pin(pin) {
        _inputCallback = cb;
        mgos_gpio_set_mode(pin, MGOS_GPIO_MODE_INPUT);
        mgos_gpio_set_int_handler_isr(pin, MGOS_GPIO_INT_EDGE_POS, int_handler, this);
    }

private:

    InputDevice::event_callback_t _inputCallback;
    static void int_handler(int pin, void *arg) {
        InputDevice *self = (InputDevice *)arg;
        self->_inputCallback(mgos_gpio_read(pin));
    }
};
