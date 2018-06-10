/*
 * Device functions, the ones exposed to outside
 *
 */

#pragma once

#include <string>

#include "mgos_rpc.h"
#include "mg_rpc_channel_loopback.h"

#include "../globals.h"
#include "../utils/dateutils.h"
#include "../network/messages.h"
#include "../network/mqtt.h"
#include "../network/topics.h"
#include "../interfaces/IOutputDevice.h"

void resetKWhCounter();
void startKWhCounter();
void read_sensors();
void ext_dev_functions_init();

#ifdef DEV_FRESNEL
#include "devfunctions.fresnel.h"
#endif

#ifdef DEV_POWR2
#include "devfunctions.powr2.h"
#endif


void turnRelay(int relayIdx, SwitchMode mode) {
    if ((size_t)relayIdx < relays.size()) {
        relays[relayIdx]->turn(mode);

        std::string powerMessage = powerFeedbackMessage(now().c_str(), (mode == SwitchMode::ON), relayIdx);
        mqttManager->publish(pubPowerFeedbackTopic, powerMessage);
    }
}


void redLED() {
    redLed->turn(SwitchMode::ON);
    mgos_usleep(1000000);
    redLed->turn(SwitchMode::OFF);
}

void greenLED() {
    greenLed->turn(SwitchMode::ON);
    mgos_usleep(1000000);
    greenLed->turn(SwitchMode::OFF);
}

void result_cb_t(struct mg_rpc *c, void *cb_arg,
                 struct mg_rpc_frame_info *fi,
                 struct mg_str result, int error_code,
                 struct mg_str error_msg) {

    (void)c;
    (void)cb_arg;
    (void)fi;
    (void)error_msg;

    if(error_code == 0 && mqttManager != nullptr) {
        char l_result[result.len + 1];
        memset(l_result, 0, result.len);
        memcpy(l_result, result.p, result.len);
        std::string jsonCrontab = std::string(l_result);
        mqttManager->publish(pubEventCrontabTopic, jsonCrontab);
    } else {
        LOG(LL_DEBUG, ("readCrontab::result_cb_t error_code = %d", error_code));
    }
}

void readCrontab() {
    struct mg_rpc_call_opts opts; // = {};
    memset(&opts, 0, sizeof(struct mg_rpc_call_opts));

    opts.dst = mg_mk_str(MGOS_RPC_LOOPBACK_ADDR);

    mg_rpc_callf(mgos_rpc_get_global(), mg_mk_str("Cron.List"),
                 result_cb_t, NULL, &opts, NULL);
}

void dev_functions_init() {
    ext_dev_functions_init();
}