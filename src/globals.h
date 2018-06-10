//
// Created by Fabrizio Guglielmino on 15/10/17.
//

#pragma once

#include "network/mqtt.h"
#include "interfaces/IOutputDevice.h"
#include "config/settings.hpp"
#include "utils/dateutils.h"
#include "hardware/devices/MCP39F511/uart/IMCP39F511UARTProto.h"


static Settings settings;

extern MQTTManager *mqttManager;

extern IOutputDevice *rele0;
extern IOutputDevice *rele1;
extern IOutputDevice *greenLed;
extern IOutputDevice *redLed;


extern IMCP39F511UARTProto *mcp39F511UARTProto;

static SwitchMode relayState = SwitchMode::OFF;
std::vector<IOutputDevice*> relays;