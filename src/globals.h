//
// Created by Fabrizio Guglielmino on 15/10/17.
//

#pragma once

#include "network/mqtt.h"
#include "hardware/gpio/OutputDevice.h"

using namespace S6MqttModule;

extern MQTTManager *mqttManager;
extern OutputDevice *rele1;
extern OutputDevice *statusLed;