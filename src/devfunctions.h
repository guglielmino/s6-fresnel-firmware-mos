/*
 * Device functions, the ones exposed to outside
 *
 */

#pragma once

#include <string>
#include "globals.h"
#include "utils/dateutils.h"
#include "network/messages.h"
#include "network/mqtt.h"
#include "network/topics.h"
#include "interfaces/IOutputDevice.h"
#include "hardware/devices/MCP39F511/MCP39F511DataTypes.hpp"
#include "hardware/devices/MCP39F511/commands/ReadMeteringValues.hpp"
#include "hardware/devices/MCP39F511/commands/StartKWhAccumulator.hpp"
#include "hardware/devices/MCP39F511/commands/ResetKWhAccumulator.hpp"


#define METERING_BUFFER_SIZE 35
#define OFF_REACTIVE_POWER 16
#define OFF_ACTIVE_POWER 12
#define OFF_CURRENT 8
#define OFF_FREQ 2
#define OFF_POWER_FACTOR 6
#define OFF_VOLTAGE 0
#define OFF_CONSUMPTION 24


void turnRelay(int relayIdx, SwitchMode mode) {
    if (relayIdx < relays.size()) {
        relays[relayIdx]->turn(mode);

        std::string powerMessage = powerFeedbackMessage(now().c_str(), (mode == SwitchMode::ON), relayIdx);
        mqttManager->publish(pubPowerFeedbackTopic, powerMessage);
    }
}

void sendValues(const uint8_t *buffer, int buffer_len) {
    if (buffer_len == METERING_BUFFER_SIZE && mqttManager != nullptr) {

        MCP39F511DataTypes dataTypes;
        int16_t raw16;
        uint32_t raw32;
        uint64_t raw64;

        raw32 = dataTypes.u32(buffer, OFF_REACTIVE_POWER);
        std::string reactiveMsg = makeSensorValueMessage(now().c_str(), REG_REACTIVE_POWER.rawToValue(raw32), "VA");
        mqttManager->publish(pubSensReactivePowerTopic, reactiveMsg);


        raw32 = dataTypes.u32(buffer, OFF_ACTIVE_POWER);
        std::string activePowerMsg = makeSensorValueMessage(now().c_str(), REG_ACTIVE_POWER.rawToValue(raw32), "W");
        mqttManager->publish(pubSensPowerTopic, activePowerMsg);


        raw32 = dataTypes.u32(buffer, OFF_CURRENT);
        std::string currentMsg = makeSensorValueMessage(now().c_str(), REG_CURRENT.rawToValue(raw32), "A");
        mqttManager->publish(pubSensCurrentTopic, currentMsg);


        raw16 = dataTypes.u16(buffer, OFF_FREQ);
        std::string freqMsg = makeSensorValueMessage(now().c_str(), REG_FREQUENCY.rawToValue(raw16), "Hz");
        mqttManager->publish(pubSensFreqTopic, freqMsg);


        raw16 = dataTypes.u16(buffer, OFF_POWER_FACTOR);
        std::string powerFactorMsg = makeSensorValueMessage(now().c_str(), REG_POWER_FACTOR.rawToValue(raw16), "");
        mqttManager->publish(pubSensPowerFactorTopic, powerFactorMsg);


        raw16 = dataTypes.u16(buffer, OFF_VOLTAGE);
        std::string voltageMsg = makeSensorValueMessage(now().c_str(), REG_VOLTAGE.rawToValue(raw16), "V");
        mqttManager->publish(pubSensVoltageTopic, voltageMsg);


        raw64 = dataTypes.u64(buffer, OFF_CONSUMPTION);
        std::string dailyConsumeMsg = makeSensorValueMessage(now().c_str(), REG_IMP_ACTIVE_CNT.rawToValue(raw64),
                                                             "KWh");
        mqttManager->publish(pubSensDailyKwhTopic, dailyConsumeMsg);
    } else {
        char dbg[50];
        scanf(dbg, "buffer len= %d", buffer_len);
        mqttManager->publish("building/debug", dbg);
    }
}

const auto readAsyncLambda = [&](RespType respType, const uint8_t *buffer, size_t size) {
    if (respType == RespType::Ack) {
        sendValues(static_cast<const uint8_t *>(buffer), (int) size);
    }
};

void read_sensors() {
    mcp39F511UARTProto->readAsync(readAsyncLambda);
    ReadMeteringValues meteringCommand;
    mcp39F511UARTProto->sendCommand(meteringCommand);
}

void resetKWhCounter() {
    ResetKWhAccumulator resetKwh;
    mcp39F511UARTProto->sendCommand(resetKwh);

}

void startKWhCounter() {
    StartKWhAccumulator startKwh;
    mcp39F511UARTProto->sendCommand(startKwh);
}

void redLED() {
    redLed->turn(SwitchMode::ON);
    mgos_usleep(1000000);
    redLed->turn(SwitchMode::OFF);
}

void greenLED() {
    greenLed->turn(SwitchMode::ON);
    mgos_usleep(1000000);
    greenLed->turn(SwitchMode::OFF);
}