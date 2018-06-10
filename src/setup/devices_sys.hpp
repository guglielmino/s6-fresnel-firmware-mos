//
// Created by Fabrizio Guglielmino on 23/12/17.
//

#pragma once

#include "../consts.h"
#include "../globals.h"
#include "../interfaces/IOutputDevice.h"
#include "../hardware/gpio/OutputDevice.h"
#include "../hardware/gpio/S6Relay.hpp"
#include "../hardware/gpio/InputDevice.h"

#include "../factories/uartFactories.h"
#include "../core/devfunctions.h"

IOutputDevice *rele0 = nullptr;
IOutputDevice *rele1 = nullptr;
IOutputDevice *redLed = nullptr;
IOutputDevice *greenLed = nullptr;
InputDevice *button = nullptr;

void ext_device_sys_init();

#ifdef DEV_FRESNEL
#include "devices_sys.fresnel.hpp"
#endif

#ifdef DEV_POWR2
#include "devices_sys.powr2.hpp"
#endif

void devices_sys_init() {
    ext_device_sys_init();

    rele0 = new S6Relay(REL0_PIN_SET, REL0_PIN_RESET);
    rele1 = new OutputDevice(REL1_PIN);
    relays.reserve(2);
    relays.push_back(rele0);
    relays.push_back(rele1);

    redLed = new OutputDevice(LED_RED_PIN);
    greenLed = new OutputDevice(LED_GREEN_PIN);

    button = new InputDevice(BUTTON_PIN, [] (bool newPinSate) {
        (void) newPinSate;
        relayState = (relayState == SwitchMode::ON ? SwitchMode::OFF : SwitchMode::ON);
        turnRelay(0, relayState);
    }, true);
}