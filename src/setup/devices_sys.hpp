//
// Created by Fabrizio Guglielmino on 23/12/17.
//

#pragma once

#include "../globals.h"
#include "../hardware/gpio/OutputDevice.h"
#include "../hardware/gpio/InputDevice.h"
#include "../devfunctions.h"

OutputDevice *rele1 = nullptr;
OutputDevice *statusLed = nullptr;
InputDevice *button = nullptr;

void devices_sys_init() {
    rele1 = new OutputDevice(REL_PIN);
    statusLed = new OutputDevice(STATUS_LED_PIN);

    button = new InputDevice(BUTTON_PIN, [] (bool newPinSate) {
        (void) newPinSate;
        relayState = (relayState == OutputDevice::ON ? OutputDevice::OFF : OutputDevice::ON);
        turnRelay(relayState);
    }, true);
}