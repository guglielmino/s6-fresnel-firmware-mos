#include "mgos.h"
#include "mgos_timers.h"
#include "mgos_config.h"
#include <time.h>

#include "consts.h"
#include "network/topics.h"
#include "utils/dateutils.h"
#include "network/messages.h"
#include "network/mqtt.h"
#include "config/settings.h"
#include "hardware/S6FPowerSensor.h"
#include "hardware/gpio/OutputDevice.h"
#include "hardware/gpio/InputDevice.h"
#include "factories/sensorsFactories.h"

using namespace S6MqttModule;

#define MAX_MSG_LEN 128

Settings settings;

IScalarSensor<float> *powerSensor = nullptr;
IScalarSensor<unsigned long> *dailyKwh = nullptr;
MQTTManager *mqttManager = nullptr;
OutputDevice *rele1 = nullptr;
OutputDevice *statusLed = nullptr;

InputDevice button(BUTTON_PIN, [] (bool newPinSate) {
    LOG(LL_DEBUG, ("Button pressed %d", (int)newPinSate));
});

auto powerSwitchSubscription = [](const char *topic, size_t topic_len, const char *msg, size_t msg_len) {
    char localTopic[MAX_TOPIC_LEN] = "";
    char localMsg[MAX_MSG_LEN] = "";
    memcpy(localTopic, topic, MIN(topic_len, MAX_TOPIC_LEN));
    memcpy(localMsg, msg, MIN(msg_len, MAX_MSG_LEN));
    LOG(LL_DEBUG, ("hander => PowerSwitch %s -> %s (REL PIN %d)", localTopic, localMsg, REL_PIN));

    OutputDevice::SwitchMode state = (strcmp(localMsg, "on") == 0 ? OutputDevice::ON : OutputDevice::OFF);
    LOG(LL_DEBUG, ("STATE =>  %d", (int)state));

    rele1->turn(state);
    statusLed->turn(state);

    char powerMessage[MQTT_MESSAGE_SIZE];

    powerFeedbackMessage(powerMessage, sizeof(powerMessage), (state == OutputDevice::ON ));
    mqttManager->publish(pubPowerFeedbackTopic, powerMessage, strlen(powerMessage));
};

/**
 * Read power callback, called on every update_interval milliseconds (setting)
 */
void power_read_timed(void *) {
    float powerValue = powerSensor->readValue();
    unsigned long dailyConsume = dailyKwh->readValue();

    if (mqttManager != nullptr) {
        char messageBuffer[100];
        powerConsumeMessage(messageBuffer, sizeof(messageBuffer), now().c_str(), powerValue);
        mqttManager->publish(pubSensPowerTopic, (const char *) messageBuffer, strlen(messageBuffer));

        memset(messageBuffer, 0, 100);
        dailyConsumeMessage(messageBuffer, sizeof(messageBuffer), now().c_str(), dailyConsume);
        mqttManager->publish(pubSensDailyKwhTopic, (const char *) messageBuffer, strlen(messageBuffer));
    }
}

void publishInfoMessage() {
     char infoMessage[100] = "";
     devInfoMessage(infoMessage, sizeof(infoMessage), FIRMWARE_APP_NAME, FIRMWARE_APP_VERSION,
                   settings.s6fresnel().location(),
                   settings.s6fresnel().name());
    mqttManager->publish(pubInfoTopic, infoMessage, strlen(infoMessage));
}



enum mgos_app_init_result mgos_app_init(void) {
    cs_log_set_level(LL_DEBUG);
    LOG(LL_DEBUG, ("Device ID %s", settings.deviceId()));

    // On board devices
    rele1 = new OutputDevice(REL_PIN);
    statusLed = new OutputDevice(STATUS_LED_PIN);

    // ** MQTT
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

    powerSensor = getPowerSensor();
    dailyKwh = getDailyKwhSensor();

    return MGOS_APP_INIT_SUCCESS;
}
