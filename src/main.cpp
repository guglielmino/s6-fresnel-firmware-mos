#include "mgos.h"
#include "mgos_timers.h"
#include "mgos_config.h"
#include <time.h>

#include "version.h"
#include "network/topics.h"
#include "utils/datetime.h"
#include "network/messages.h"
#include "network/mqtt.h"
#include "config/settings.h"
#include "hardware/io/ESP32ADCReader.h"
#include "hardware/PowerSensor.h"

using namespace S6MqttModule;

Settings settings;
ESP32ADCReader *adcReader;
PowerSensor *powerSensor;
MQTTManager *mqttManager = NULL;

/**
 * Read power callback, called on every update_interval milliseconds (setting)
 */
void power_read_timed(void *) {
    float powerValue = powerSensor->readValue();
    LOG(LL_DEBUG, ("Timed func %f", powerValue));
    if(mqttManager != NULL) {
        char powerMessage[100];
        powerConsumeMessage(powerMessage, sizeof(powerMessage), now().c_str(), powerValue);
        mqttManager->publish(pubSensPowerTopic, (const char*)powerMessage, strlen(powerMessage));
    }
}



enum mgos_app_init_result mgos_app_init(void) {
    cs_log_set_level(LL_DEBUG);
    LOG(LL_DEBUG, ("Device ID %s", settings.deviceId()));

    // ** MQTT
    makeDeviceTopic(pubSensPowerTopic, MAX_TOPIC_LEN, PUB_SENS_POWER_TOPIC, settings.s6fresnel().location(), settings.deviceId());
    mqttManager = new MQTTManager();
    mqttManager->setEventCallback(MQTTManager::Connected, []() {
        LOG(LL_DEBUG, ("S6 Fresnel:: MQTT Connected"));
        // Start periodic power publishing on MQTT topic
        mgos_set_timer(settings.s6fresnel().updateInterval(), true, power_read_timed, NULL);
    });
    // **

    adcReader = new ESP32ADCReader();
    powerSensor = new PowerSensor(adcReader);

    return MGOS_APP_INIT_SUCCESS;
}
