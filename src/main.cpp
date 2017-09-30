#include "mgos.h"
#include "mgos_timers.h"
#include "mgos_config.h"
#include <time.h>

#include "consts.h"
#include "network/topics.h"
#include "utils/datetime.h"
#include "network/messages.h"
#include "network/mqtt.h"
#include "config/settings.h"
#include "hardware/io/ESP32ADCReader.h"
#include "hardware/PowerSensor.h"
//#include "hardware/gpio/OutputDevice.h"

using namespace S6MqttModule;

#define MAX_MSG_LEN 128

Settings settings;
ESP32ADCReader *adcReader;
PowerSensor *powerSensor;
MQTTManager *mqttManager = NULL;
OutputDevice rele1(32);

auto powerSwitchSubscription = [](const char *topic, size_t topic_len, const char *msg, size_t msg_len) {
    char localTopic[MAX_TOPIC_LEN] = "";
    char localMsg[MAX_MSG_LEN] = "";
    memcpy(localTopic, topic, MIN(topic_len, MAX_TOPIC_LEN));
    memcpy(localMsg, msg, MIN(msg_len, MAX_MSG_LEN));
    LOG(LL_DEBUG, ("hander => PowerSwitch %s -> %s", localTopic, localMsg));
    //rele1.toggle();
    rele1.turn((strcmp(localMsg, "on") == 0) ? OutputDevice::ON : OutputDevice::OFF);
};

/**
 * Read power callback, called on every update_interval milliseconds (setting)
 */
void power_read_timed(void *) {
    float powerValue = powerSensor->readValue();

    if (mqttManager != NULL) {
        char powerMessage[100];
        powerConsumeMessage(powerMessage, sizeof(powerMessage), now().c_str(), powerValue);
        mqttManager->publish(pubSensPowerTopic, (const char *) powerMessage, strlen(powerMessage));
    }
}

void publishInfoMessage() {
     char infoMessage[100] = "";
     devInfoMessage(infoMessage, FIRMWARE_APP_NAME, FIRMWARE_APP_VERSION,
                   settings.s6fresnel().location(),
                   settings.s6fresnel().name());
    mqttManager->publish(pubInfoTopic, infoMessage, strlen(infoMessage));
}



enum mgos_app_init_result mgos_app_init(void) {
    cs_log_set_level(LL_DEBUG);
    LOG(LL_DEBUG, ("Device ID %s", settings.deviceId()));

    // ** MQTT
    makeDeviceTopic(pubSensPowerTopic, MAX_TOPIC_LEN, PUB_SENS_POWER_TOPIC, settings.s6fresnel().location(),
                    settings.deviceId());
    makeDeviceTopic(subSwitchDevTopic, MAX_TOPIC_LEN, SUB_SWITCH_DEV, settings.s6fresnel().location(),
                    settings.deviceId());
    makeRoomTopic(subSwitchRoomTopic, MAX_TOPIC_LEN, SUB_SWITCH_ROOM, settings.s6fresnel().location());

    makeDeviceTopic(pubInfoTopic, MAX_TOPIC_LEN, PUB_SENS_INFO_TOPIC, settings.s6fresnel().location(),
                    settings.deviceId());

    mqttManager = new MQTTManager();

    mqttManager->setEventCallback(MQTTManager::Connected, []() {
        LOG(LL_DEBUG, ("S6 Fresnel:: MQTT Connected"));

        // Subscribe topics
        mqttManager->subcribe(subSwitchDevTopic, powerSwitchSubscription);
        mqttManager->subcribe(subSwitchRoomTopic, powerSwitchSubscription);

        publishInfoMessage();

        // Start periodic power publishing on MQTT topic
        mgos_set_timer(settings.s6fresnel().updateInterval(), true, power_read_timed, NULL);
    });

    mqttManager->setEventCallback(MQTTManager::Disconnected, []() {
        LOG(LL_DEBUG, ("S6 Fresnel:: MQTT Disconnected"));
    });


    mqttManager->setEventCallback(MQTTManager::Subscribe, []() {
        LOG(LL_DEBUG, ("S6 Fresnel:: MQTT Subscribe"));
    });


    // **

    adcReader = new ESP32ADCReader();
    powerSensor = new PowerSensor(adcReader);

    return MGOS_APP_INIT_SUCCESS;
}
