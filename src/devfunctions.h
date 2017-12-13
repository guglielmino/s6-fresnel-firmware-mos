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
        if(reactiveValue.isValid()) {
            std::string reactiveMsg = makeSensorValueMessage(now().c_str(), reactiveValue.value(), "VA");
            mqttManager->publish(pubSensReactivePowerTopic, reactiveMsg);
        }

        SensorValue<float> powerValue = activePower->readValue();
        if(powerValue.isValid()) {
            std::string activePowerMsg = makeSensorValueMessage(now().c_str(), powerValue.value(), "W");
            mqttManager->publish(pubSensPowerTopic, activePowerMsg);
        }

        SensorValue<float> dailyConsume = dailyKwh->readValue();
        if(dailyConsume.isValid()) {
            std::string dailyConsumeMsg = makeSensorValueMessage(now().c_str(), dailyConsume.value(), "KWh");
            mqttManager->publish(pubSensDailyKwhTopic, dailyConsumeMsg);
        }

        SensorValue<float> currentValue = current->readValue();
        if(currentValue.isValid()) {
            std::string currentMsg = makeSensorValueMessage(now().c_str(), currentValue.value(), "A");
            mqttManager->publish(pubSensCurrentTopic, currentMsg);
        }

        SensorValue<float> freqValue = frequency->readValue();
        if(freqValue.isValid()) {
            std::string freqMsg = makeSensorValueMessage(now().c_str(), freqValue.value(), "Hz");
            mqttManager->publish(pubSensFreqTopic, freqMsg);
        }

        SensorValue<float> powerFactorValue = powerFactor->readValue();
        if(powerFactorValue.isValid()) {
            std::string powerFactorMsg = makeSensorValueMessage(now().c_str(), powerFactorValue.value(), "");
            mqttManager->publish(pubSensPowerFactorTopic, powerFactorMsg);
        }

        SensorValue<float> voltageValue = voltage->readValue();
        if(voltageValue.isValid()) {
            std::string voltageMsg = makeSensorValueMessage(now().c_str(), voltageValue.value(), "V");
            mqttManager->publish(pubSensVoltageTopic, voltageMsg);
        }
    }
}