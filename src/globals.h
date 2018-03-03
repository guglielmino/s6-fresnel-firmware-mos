//
// Created by Fabrizio Guglielmino on 15/10/17.
//

#pragma once

#include "network/mqtt.h"
#include "interfaces/IOutputDevice.h"
#include "config/settings.h"
#include "interfaces/IScalarSensor.h"
#include "hardware/sensors/SensorValue.hpp"
#include "utils/dateutils.h"

extern Settings settings;

extern MQTTManager *mqttManager;

extern IOutputDevice *rele0;
extern IOutputDevice *rele1;
extern IOutputDevice *greenLed;
extern IOutputDevice *redLed;

extern IScalarSensor<SensorValue<float>> *activePower;
extern IScalarSensor<SensorValue<float>> *reactivePower;
extern IScalarSensor<SensorValue<float>> *dailyKwh;
extern IScalarSensor<SensorValue<float>> *current;
extern IScalarSensor<SensorValue<float>> *frequency;
extern IScalarSensor<SensorValue<float>> *powerFactor;
extern IScalarSensor<SensorValue<float>> *voltage;

static SwitchMode relayState = SwitchMode::OFF;
std::vector<IOutputDevice*> relays;