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

    // Add the first item by default (read sensor data)
    // TODO: Check is it can be done by embedding the JSON file in the fs folder

    struct mg_str action = MG_NULL_STR;
    char *error;
    mgos_crontab_job_id_t pid;

    bool cronPid1 = mgos_crontab_job_get(1, NULL, NULL, &action, NULL, &error);

    if (!cronPid1) {
        LOG(LL_DEBUG, ("cron_sys_init: READ CRON TAB ITEM 1 error %s", error));
        char cronexpr[20] = "";
        snprintf(cronexpr, 20, "*/%d * * * * *", settings.s6fresnel().updateInterval() / 1000);
        if (mgos_crontab_job_add(mg_mk_str(cronexpr), true, mg_mk_str("readSensors"), MG_NULL_STR, &pid, &error)) {
            LOG(LL_DEBUG, ("cron_sys_init: JOB ADDED"));
        }
    }

}

