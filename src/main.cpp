#include <string>
#include <time.h>

#include "mgos.h"
#include "mgos_timers.h"
#include "mgos_config.h"


#include "consts.h"
#include "network/topics.h"
#include "utils/dateutils.h"
#include "network/messages.h"
#include "network/mqtt.h"
#include "config/settings.h"
#include "hardware/gpio/OutputDevice.h"
#include "hardware/gpio/InputDevice.h"
#include "factories/sensorsFactories.h"
#include "hardware/io/UARTInterface.hpp"

#include "globals.h"
#include "devfunctions.h"

using namespace S6MqttModule;

#define MAX_MSG_LEN 128

Settings settings;

IScalarSensor<float> *activePower = nullptr;
IScalarSensor<float> *reactivePower = nullptr;
IScalarSensor<float> *dailyKwh = nullptr;
IScalarSensor<float> *current = nullptr;
IScalarSensor<float> *frequency = nullptr;
IScalarSensor<float> *powerFactor = nullptr;

MQTTManager *mqttManager = nullptr;
OutputDevice *rele1 = nullptr;
OutputDevice *statusLed = nullptr;

InputDevice *button = nullptr;


OutputDevice::SwitchMode relayState =  OutputDevice::OFF;

auto powerSwitchSubscription = [](const char *topic, size_t topic_len, const char *msg, size_t msg_len) {
    char localMsg[MAX_MSG_LEN] = "";
    memcpy(localMsg, msg, MIN(msg_len, MAX_MSG_LEN));
    (void)topic;
    (void)topic_len;

    relayState = (strcmp(localMsg, "on") == 0 ? OutputDevice::ON : OutputDevice::OFF);

    turnRelay(relayState);
};

/**
 * Read power callback, called on every update_interval milliseconds (setting)
 */
void power_read_timed(void *) {

    if (mqttManager != nullptr) {
        float powerValue = activePower->readValue();
        std::string powerConsumeMsg = makeSensorValueMessage(now().c_str(), powerValue, "W");
        mqttManager->publish(pubSensPowerTopic, powerConsumeMsg);

        float reactiveValue = reactivePower->readValue();
        std::string reactiveMsg = makeSensorValueMessage(now().c_str(), reactiveValue, "VA");
        mqttManager->publish(pubSensReactivePowerTopic, reactiveMsg);

        float dailyConsume = dailyKwh->readValue();
        std::string dailyConsumeMsg = makeSensorValueMessage(now().c_str(), dailyConsume, "KWh");
        mqttManager->publish(pubSensDailyKwhTopic, dailyConsumeMsg);

        float currentValue = current->readValue();
        std::string currentMsg = makeSensorValueMessage(now().c_str(), currentValue, "A");
        mqttManager->publish(pubSensCurrentTopic, currentMsg);

        float freqValue = frequency->readValue();
        std::string freqMsg = makeSensorValueMessage(now().c_str(), freqValue, "Hz");
        mqttManager->publish(pubSensFreqTopic, freqMsg);

        float powerFactorValue = powerFactor->readValue();
        std::string powerFactorMsg = makeSensorValueMessage(now().c_str(), powerFactorValue, "");
        mqttManager->publish(pubSensPowerFactorTopic, powerFactorMsg);
    }
}

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



enum mgos_app_init_result mgos_app_init(void) {
    cs_log_set_level(LL_DEBUG);
    LOG(LL_DEBUG, ("Device ID %s", settings.deviceId()));

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

        // Start periodic power publishing on MQTT topic
        mgos_set_timer(settings.s6fresnel().updateInterval(), true, power_read_timed, NULL);
    });

    mqttManager->setEventCallback(MQTTManager::Disconnected, []() {
        LOG(LL_DEBUG, ("S6 Fresnel:: MQTT Disconnected"));
    });


    mqttManager->setEventCallback(MQTTManager::Subscribe, []() {
        LOG(LL_DEBUG, ("S6 Fresnel:: MQTT Subscribe"));
    });

    // SETUP: On board devices
    rele1 = new OutputDevice(REL_PIN);
    statusLed = new OutputDevice(STATUS_LED_PIN);
    activePower = getActivePowerSensor();
    reactivePower = getReactivePowerSensor();
    dailyKwh = getDailyKwhSensor();
    current = getCurrentSensor();
    frequency = getLineFrequencySensor();
    powerFactor = getPowerFactorSensor();

    ISensorCommand *startDailyKwhCounter = getStartDailyKkhCommand();
    startDailyKwhCounter->exec();

    button = new InputDevice(BUTTON_PIN, [] (bool newPinSate) {
        (void) newPinSate;
        relayState = (relayState == OutputDevice::ON ? OutputDevice::OFF : OutputDevice::ON);
        turnRelay(relayState);
    }, true);

    return MGOS_APP_INIT_SUCCESS;
}
