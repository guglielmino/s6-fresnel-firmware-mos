//
// Created by Fabrizio Guglielmino on 09/12/17.
//
// Cron System initialization


#pragma once

#include "mgos_crontab.h"
#include "../interfaces/IOutputDevice.h"
#include "../core/devfunctions.h"
#include "../config/settings.hpp"

// CRON FUNCTIONS
void relayOn(struct mg_str action, struct mg_str payload, void *userdata) {
    (void)action;
    (void)userdata;

    int relay_idx = 0;
    json_scanf(payload.p, payload.len, "{ relay_idx: %d }", &relay_idx);
    turnRelay(relay_idx, SwitchMode::ON);
    LOG(LL_DEBUG, ("CRON: relay %d on", relay_idx));
}

void relayOff(struct mg_str action, struct mg_str payload, void *userdata) {
    (void)action;
    (void)userdata;

    int relay_idx = 0;
    json_scanf(payload.p, payload.len, "{ relay_idx: %d }", &relay_idx);

    turnRelay(relay_idx, SwitchMode::OFF);
    LOG(LL_DEBUG, ("CRON: relayOff"));
}

void readSensors(struct mg_str action, struct mg_str payload, void *userdata) {
    (void)action;
    (void)payload;
    (void)userdata;
    read_sensors();
    LOG(LL_DEBUG, ("CRON: readSensors"));
}

void resetKWh(struct mg_str action, struct mg_str payload, void *userdata) {
    (void)action;
    (void)payload;
    (void)userdata;
    resetKWhCounter();
    startKWhCounter();
    LOG(LL_DEBUG, ("CRON: reset KWh counter"));
}

void redLED(struct mg_str action, struct mg_str payload, void *userdata) {
    (void)action;
    (void)payload;
    (void)userdata;
    redLED();
    LOG(LL_DEBUG, ("CRON: red led"));
}

void greenLED(struct mg_str action, struct mg_str payload, void *userdata) {
    (void)action;
    (void)payload;
    (void)userdata;
    redLED();
    LOG(LL_DEBUG, ("CRON: green led"));
}


void cron_sys_init() {
    mgos_crontab_register_handler(mg_mk_str("relayOn"), relayOn, NULL);
    mgos_crontab_register_handler(mg_mk_str("relayOff"), relayOff, NULL);
    mgos_crontab_register_handler(mg_mk_str("readSensors"), readSensors, NULL);
    mgos_crontab_register_handler(mg_mk_str("resetKWh"), resetKWh, NULL);
    mgos_crontab_register_handler(mg_mk_str("redLED"), redLED, NULL);
    mgos_crontab_register_handler(mg_mk_str("greenLED"), greenLED, NULL);
}

