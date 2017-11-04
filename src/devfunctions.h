/*
 * Device functions, the ones exposed to outside
 *
 */

#pragma once

#include <string>
#include "globals.h"
#include "network/messages.h"
#include "network/mqtt.h"
#include "hardware/gpio/OutputDevice.h"

void turnRelay(OutputDevice::SwitchMode mode) {
    rele1->turn(mode);
    statusLed->turn(mode);

    std::string powerMessage = powerFeedbackMessage((mode == OutputDevice::ON ));
    mqttManager->publish(pubPowerFeedbackTopic, powerMessage.c_str(), powerMessage.size());
}