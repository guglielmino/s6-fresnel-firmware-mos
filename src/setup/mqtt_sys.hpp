//
// Created by Fabrizio Guglielmino on 23/12/17.
//

#pragma once

#include "../globals.h"
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

    relayState = (strcmp(localMsg, "on") == 0 ? SwitchMode::ON : SwitchMode::OFF);

    turnRelay(relayState);
};


void publishInfoMessage() {
    std::string infoMessage = devInfoMessage(FIRMWARE_APP_NAME, FIRMWARE_APP_VERSION,
                                             settings.s6fresnel().location(),
                                             settings.s6fresnel().name());
    mqttManager->publish(pubInfoTopic, infoMessage);
}

void publishLWTOnlineMessage(bool online) {
    std::string message = lwtMessage(online);
    mqttManager->publish(pubLWTTopic, message);
}

void mqtt_sys_init() {
    // ** MQTT Topic (TODO: Move to std::string and optimize topic string creation)
    makeDeviceTopic(pubSensPowerTopic, MAX_TOPIC_LEN, PUB_SENS_POWER_TOPIC, settings.s6fresnel().location(),
                    settings.deviceId());
    makeDeviceTopic(subSwitchDevTopic, MAX_TOPIC_LEN, SUB_SWITCH_DEV, settings.s6fresnel().location(),
                    settings.deviceId());
    makeRoomTopic(subSwitchRoomTopic, MAX_TOPIC_LEN, SUB_SWITCH_ROOM, settings.s6fresnel().location());

    makeDeviceTopic(pubInfoTopic, MAX_TOPIC_LEN, PUB_SENS_INFO_TOPIC, settings.s6fresnel().location(),
                    settings.deviceId());

    makeDeviceTopic(pubSensDailyKwhTopic, MAX_TOPIC_LEN, PUB_SENS_DAILYKWH_TOPIC, settings.s6fresnel().location(),
                    settings.deviceId());

    makeDeviceTopic(pubPowerFeedbackTopic, MAX_TOPIC_LEN, PUB_EVENT_POWERFEEDBACK_TOPIC, settings.s6fresnel().location(),
                    settings.deviceId());

    makeDeviceTopic(pubLWTTopic, MAX_TOPIC_LEN, PUB_EVENT_LWT_TOPIC, settings.s6fresnel().location(),
                    settings.deviceId());

    makeDeviceTopic(pubSensCurrentTopic, MAX_TOPIC_LEN, PUB_SENS_CURRENT_TOPIC, settings.s6fresnel().location(),
                    settings.deviceId());

    makeDeviceTopic(pubSensFreqTopic, MAX_TOPIC_LEN, PUB_SENS_FREQUENCY_TOPIC, settings.s6fresnel().location(),
                    settings.deviceId());

    makeDeviceTopic(pubSensReactivePowerTopic, MAX_TOPIC_LEN, PUB_SENS_RPOWER_TOPIC, settings.s6fresnel().location(),
                    settings.deviceId());

    makeDeviceTopic(pubSensPowerFactorTopic, MAX_TOPIC_LEN, PUB_SENS_POWERFACTOR_TOPIC, settings.s6fresnel().location(),
                    settings.deviceId());

    makeDeviceTopic(pubSensVoltageTopic, MAX_TOPIC_LEN, PUB_SENS_VOLTAGE_TOPIC, settings.s6fresnel().location(),
                    settings.deviceId());

    std::string message = lwtMessage(false);
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