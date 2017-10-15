/*
 * Device functions, the ones exposed to outside
 *
 */

#pragma once

#include "globals.h"
#include "network/messages.h"
#include "network/mqtt.h"
#include "hardware/gpio/OutputDevice.h"

void turnRelay(OutputDevice::SwitchMode mode) {
    rele1->turn(mode);
    statusLed->turn(mode);

    char powerMessage[MQTT_MESSAGE_SIZE];

    powerFeedbackMessage(powerMessage, sizeof(powerMessage), (mode == OutputDevice::ON ));
    mqttManager->publish(pubPowerFeedbackTopic, powerMessage, strlen(powerMessage));
}