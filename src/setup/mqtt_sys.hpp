//
// Created by Fabrizio Guglielmino on 23/12/17.
//

#pragma once

#include "../globals.h"
#include "../utils/dateutils.h"
#include "../network/mqtt.h"
#include "../network/topics.h"

#include "../devfunctions.h"

MQTTManager *mqttManager = nullptr;

#define MAX_MSG_LEN 128

auto powerSwitchSubscription = [](const char *topic, size_t topic_len, const char *msg, size_t msg_len) {
    char localMsg[MAX_MSG_LEN] = "";
    memcpy(localMsg, msg, MIN(msg_len, MAX_MSG_LEN));
    (void)topic;
    (void)topic_len;

    SwitchMode relayState =  SwitchMode::OFF;
    int relay_idx = -1;
    if(msg_len > 8) {
        char *operation;
        json_scanf(localMsg, strlen(localMsg), "{ relay_idx:%d, op:%Q }", &relay_idx, &operation);
        relayState = (strcmp(operation, "on") == 0 ? SwitchMode::ON : SwitchMode::OFF);
    } else {
        relay_idx = 0;
        relayState = (strcmp(localMsg, "on") == 0 ? SwitchMode::ON : SwitchMode::OFF);
    }

    turnRelay(relay_idx, relayState);
};


void publishInfoMessage() {
    std::string infoMessage = devInfoMessage(now().c_str(), FIRMWARE_APP_NAME, FIRMWARE_APP_VERSION,
                                             settings.s6fresnel().group(),
                                             settings.s6fresnel().name(), settings.s6fresnel().features());
    mqttManager->publish(pubInfoTopic, infoMessage);
}

void publishLWTOnlineMessage(bool online) {
    std::string message = lwtMessage(now().c_str(), online);
    mqttManager->publish(pubLWTTopic, message);
}

void mqtt_sys_init() {
    // ** MQTT Topic (TODO: Move to std::string and optimize topic string creation)
    makeDeviceTopic(pubSensPowerTopic, MAX_TOPIC_LEN, PUB_SENS_POWER_TOPIC, settings.s6fresnel().group(),
                    settings.deviceId());
    makeDeviceTopic(subSwitchDevTopic, MAX_TOPIC_LEN, SUB_SWITCH_DEV, settings.s6fresnel().group(),
                    settings.deviceId());
    makeRoomTopic(subSwitchRoomTopic, MAX_TOPIC_LEN, SUB_SWITCH_ROOM, settings.s6fresnel().group());

    makeDeviceTopic(pubInfoTopic, MAX_TOPIC_LEN, PUB_SENS_INFO_TOPIC, settings.s6fresnel().group(),
                    settings.deviceId());

    makeDeviceTopic(pubSensDailyKwhTopic, MAX_TOPIC_LEN, PUB_SENS_DAILYKWH_TOPIC, settings.s6fresnel().group(),
                    settings.deviceId());

    makeDeviceTopic(pubPowerFeedbackTopic, MAX_TOPIC_LEN, PUB_EVENT_POWERFEEDBACK_TOPIC, settings.s6fresnel().group(),
                    settings.deviceId());

    makeDeviceTopic(pubLWTTopic, MAX_TOPIC_LEN, PUB_EVENT_LWT_TOPIC, settings.s6fresnel().group(),
                    settings.deviceId());

    makeDeviceTopic(pubSensCurrentTopic, MAX_TOPIC_LEN, PUB_SENS_CURRENT_TOPIC, settings.s6fresnel().group(),
                    settings.deviceId());

    makeDeviceTopic(pubSensFreqTopic, MAX_TOPIC_LEN, PUB_SENS_FREQUENCY_TOPIC, settings.s6fresnel().group(),
                    settings.deviceId());

    makeDeviceTopic(pubSensReactivePowerTopic, MAX_TOPIC_LEN, PUB_SENS_RPOWER_TOPIC, settings.s6fresnel().group(),
                    settings.deviceId());

    makeDeviceTopic(pubSensPowerFactorTopic, MAX_TOPIC_LEN, PUB_SENS_POWERFACTOR_TOPIC, settings.s6fresnel().group(),
                    settings.deviceId());

    makeDeviceTopic(pubSensVoltageTopic, MAX_TOPIC_LEN, PUB_SENS_VOLTAGE_TOPIC, settings.s6fresnel().group(),
                    settings.deviceId());

    std::string message = lwtMessage(now().c_str(), false);
    settings.mqtt().lwtMessage(message);
    settings.mqtt().lwtTopic(pubLWTTopic);


    mqttManager = new MQTTManager();

    mqttManager->setEventCallback(MQTTManager::Connected, []() {
        LOG(LL_DEBUG, ("S6 Fresnel:: MQTT Connected"));

        // Subscribe topics
        mqttManager->subcribe(subSwitchDevTopic, powerSwitchSubscription);
        mqttManager->subcribe(subSwitchRoomTopic, powerSwitchSubscription);

        publishInfoMessage();
        publishLWTOnlineMessage(true);
    });

    mqttManager->setEventCallback(MQTTManager::Disconnected, []() {
        LOG(LL_DEBUG, ("S6 Fresnel:: MQTT Disconnected"));
    });


    mqttManager->setEventCallback(MQTTManager::Subscribe, []() {
        LOG(LL_DEBUG, ("S6 Fresnel:: MQTT Subscribe"));
    });
}