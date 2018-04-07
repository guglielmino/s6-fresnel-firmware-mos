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

#define REACTIVE_POWER "REACTIVE_POWER"
#define ACTIVE_POWER "ACTIVE_POWER"
#define CURRENT "CURRENT"
#define FREQ "FREQ"
#define POWER_FACTOR "POWER_FACTOR"
#define VOLTAGE "VOLTAGE"
#define CONSUMPTION "CONSUMPTION"



void turnRelay(int relayIdx, SwitchMode mode) {
    if(relayIdx < relays.size()) {
        relays[relayIdx]->turn(mode);

        std::string powerMessage = powerFeedbackMessage(now().c_str(), (mode == SwitchMode::ON), relayIdx);
        mqttManager->publish(pubPowerFeedbackTopic, powerMessage);
    }
}

std::map<std::string, float> getValues() {
    std::map<std::string, float> ret = std::map<std::string, float>();

    // Throw away first reading because is always wrong
    reactivePower->readValue();
    mgos_wdt_feed();

    SensorValue<float> reactiveValue = reactivePower->readValue();
    mgos_wdt_feed();
    if(reactiveValue.isValid()) {
        ret.insert(std::pair<std::string,float>(REACTIVE_POWER, reactiveValue.value()));
    }

    SensorValue<float> powerValue = activePower->readValue();
    mgos_wdt_feed();
    if(powerValue.isValid()) {
        ret.insert(std::pair<std::string,float>(ACTIVE_POWER, powerValue.value()));
    }

    SensorValue<float> currentValue = current->readValue();
    mgos_wdt_feed();
    if(currentValue.isValid()) {
        ret.insert(std::pair<std::string,float>(CURRENT, currentValue.value()));
    }

    SensorValue<float> freqValue = frequency->readValue();
    mgos_wdt_feed();
    if(freqValue.isValid()) {
        ret.insert(std::pair<std::string,float>(FREQ, freqValue.value()));
    }

    SensorValue<float> powerFactorValue = powerFactor->readValue();
    mgos_wdt_feed();
    if(powerFactorValue.isValid()) {
        ret.insert(std::pair<std::string,float>(POWER_FACTOR, powerFactorValue.value()));
    }

    SensorValue<float> voltageValue = voltage->readValue();
    mgos_wdt_feed();
    if(voltageValue.isValid()) {
        ret.insert(std::pair<std::string,float>(VOLTAGE, voltageValue.value()));
    }

    SensorValue<float> consumptionValue = dailyKwh->readValue();
    mgos_wdt_feed();
    if(consumptionValue.isValid()) {
        ret.insert(std::pair<std::string,float>(CONSUMPTION, consumptionValue.value()));
    }

    return ret;
}

void read_sensors() {

    if (mqttManager != nullptr) {

        std::map<std::string, float> values = getValues();

        if(values[REACTIVE_POWER]) {
            std::string reactiveMsg = makeSensorValueMessage(now().c_str(), values[REACTIVE_POWER], "VA");
            mqttManager->publish(pubSensReactivePowerTopic, reactiveMsg);
        }

        if(values[ACTIVE_POWER]) {
            std::string activePowerMsg = makeSensorValueMessage(now().c_str(), values[ACTIVE_POWER], "W");
            mqttManager->publish(pubSensPowerTopic, activePowerMsg);
        }

        if(values[CURRENT]) {
            std::string currentMsg = makeSensorValueMessage(now().c_str(), values[CURRENT], "A");
            mqttManager->publish(pubSensCurrentTopic, currentMsg);
        }
        if(values[FREQ]) {
            std::string freqMsg = makeSensorValueMessage(now().c_str(), values[FREQ], "Hz");
            mqttManager->publish(pubSensFreqTopic, freqMsg);
        }

        if(values[POWER_FACTOR]) {
            std::string powerFactorMsg = makeSensorValueMessage(now().c_str(),values[POWER_FACTOR], "");
            mqttManager->publish(pubSensPowerFactorTopic, powerFactorMsg);
        }

        if(values[VOLTAGE]) {
            std::string voltageMsg = makeSensorValueMessage(now().c_str(), values[VOLTAGE], "V");
            mqttManager->publish(pubSensVoltageTopic, voltageMsg);
        }

        if(values[CONSUMPTION]) {
            std::string dailyConsumeMsg = makeSensorValueMessage(now().c_str(), values[CONSUMPTION], "KWh");
            mqttManager->publish(pubSensDailyKwhTopic, dailyConsumeMsg);
        }
    }

}

void resetKWhCounter() {
    ISensorCommand *startDailyKwhCounter = getResetDailyKkhCommand();
    startDailyKwhCounter->exec();
}

void startKWhCounter() {
    ISensorCommand *startDailyKwhCounter = getStartDailyKkhCommand();
    startDailyKwhCounter->exec();
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