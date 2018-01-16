//
// Created by Fabrizio Guglielmino on 23/12/17.
//

#pragma once

#include "../globals.h"
#include "../interfaces/IOutputDevice.h"
#include "../hardware/gpio/OutputDevice.h"
#include "../hardware/gpio/S6Relay.hpp"
#include "../hardware/gpio/InputDevice.h"
#include "../devfunctions.h"

IOutputDevice *rele1 = nullptr;
IOutputDevice *redLed = nullptr;
IOutputDevice *greenLed = nullptr;
InputDevice *button = nullptr;

void devices_sys_init() {
    rele1 = new S6Relay(REL_PIN_SET, REL_PIN_RESET);
    redLed = new OutputDevice(LED_RED_PIN);
    greenLed = new OutputDevice(LED_GREEN_PIN);

    button = new InputDevice(BUTTON_PIN, [] (bool newPinSate) {
        (void) newPinSate;
        relayState = (relayState == SwitchMode::ON ? SwitchMode::OFF : SwitchMode::ON);
        turnRelay(relayState);
    }, true);
}