/*
 * Device functions, the ones exposed to outside
 *
 */

#pragma once

#include <string>
#include "globals.h"
#include "network/messages.h"
#include "network/mqtt.h"
#include "network/topics.h"
#include "hardware/gpio/OutputDevice.h"

#define REACTIVE_POWER "REACTIVE_POWER"
#define ACTIVE_POWER "ACTIVE_POWER"
#define CURRENT "CURRENT"
#define FREQ "FREQ"
#define POWER_FACTOR "POWER_FACTOR"
#define VOLTAGE "VOLTAGE"

void turnRelay(OutputDevice::SwitchMode mode) {
    rele1->turn(mode);
    statusLed->turn(mode);

    std::string powerMessage = powerFeedbackMessage((mode == OutputDevice::ON ));
    mqttManager->publish(pubPowerFeedbackTopic, powerMessage);
}

void read_sensors() {

    if (mqttManager != nullptr) {
        // HACK: Lettuta "fake" per scartare il primo caso con dati "spuri"
        reactivePower->readValue();

        SensorValue<float> reactiveValue = reactivePower->readValue();
        mgos_wdt_feed();
        if(reactiveValue.isValid()) {
            std::string reactiveMsg = makeSensorValueMessage(now().c_str(), reactiveValue.value(), "VA");
            mqttManager->publish(pubSensReactivePowerTopic, reactiveMsg);
        }

        SensorValue<float> powerValue = activePower->readValue();
        mgos_wdt_feed();
        if(powerValue.isValid()) {
            std::string activePowerMsg = makeSensorValueMessage(now().c_str(), powerValue.value(), "W");
            mqttManager->publish(pubSensPowerTopic, activePowerMsg);
        }

        SensorValue<float> dailyConsume = dailyKwh->readValue();
        mgos_wdt_feed();
        if(dailyConsume.isValid()) {
            std::string dailyConsumeMsg = makeSensorValueMessage(now().c_str(), dailyConsume.value(), "KWh");
            mqttManager->publish(pubSensDailyKwhTopic, dailyConsumeMsg);
        }

        SensorValue<float> currentValue = current->readValue();
        mgos_wdt_feed();
        if(currentValue.isValid()) {
            std::string currentMsg = makeSensorValueMessage(now().c_str(), currentValue.value(), "A");
            mqttManager->publish(pubSensCurrentTopic, currentMsg);
        }

        SensorValue<float> freqValue = frequency->readValue();
        mgos_wdt_feed();
        if(freqValue.isValid()) {
            std::string freqMsg = makeSensorValueMessage(now().c_str(), freqValue.value(), "Hz");
            mqttManager->publish(pubSensFreqTopic, freqMsg);
        }

        SensorValue<float> powerFactorValue = powerFactor->readValue();
        mgos_wdt_feed();
        if(powerFactorValue.isValid()) {
            std::string powerFactorMsg = makeSensorValueMessage(now().c_str(), powerFactorValue.value(), "");
            mqttManager->publish(pubSensPowerFactorTopic, powerFactorMsg);
        }

        SensorValue<float> voltageValue = voltage->readValue();
        mgos_wdt_feed();
        if(voltageValue.isValid()) {
            std::string voltageMsg = makeSensorValueMessage(now().c_str(), voltageValue.value(), "V");
            mqttManager->publish(pubSensVoltageTopic, voltageMsg);
        }
    }
}

std::map<std::string, float> getValues() {
    std::map<std::string, float> ret = std::map<std::string, float>();

    // Trash first read
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

    return ret;
}