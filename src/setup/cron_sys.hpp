//
// Created by Fabrizio Guglielmino on 09/12/17.
//
// Cron System initialization


#pragma once

#include "mgos_crontab.h"
#include "../devfunctions.h"
#include "../config/settings.h"

// CRON FUNCTIONS
void relayOn(struct mg_str action, struct mg_str payload, void *userdata) {
    turnRelay(OutputDevice::ON);
    LOG(LL_DEBUG, ("CRON: relayOn"));
}

void relayOff(struct mg_str action, struct mg_str payload, void *userdata) {
    turnRelay(OutputDevice::OFF);
    LOG(LL_DEBUG, ("CRON: relayOff"));
}

void readSensors(struct mg_str action, struct mg_str payload, void *userdata) {
    read_sensors();
    LOG(LL_DEBUG, ("CRON: readSensors"));
}


void cron_sys_init() {
    mgos_crontab_register_handler(mg_mk_str("relayOn"), relayOn, NULL);
    mgos_crontab_register_handler(mg_mk_str("relayOff"), relayOff, NULL);
    mgos_crontab_register_handler(mg_mk_str("readSensors"), readSensors, NULL);
}

