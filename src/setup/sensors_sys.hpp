//
// Created by Fabrizio Guglielmino on 12/12/17.
//

#pragma once

#include "../globals.h"
#include "../interfaces/IScalarSensor.h"
#include "../hardware/sensors/SensorValue.hpp"
#include "../factories/sensorsFactories.h"

IScalarSensor<SensorValue<float>> *activePower = nullptr;
IScalarSensor<SensorValue<float>> *reactivePower = nullptr;
IScalarSensor<SensorValue<float>> *dailyKwh = nullptr;
IScalarSensor<SensorValue<float>> *current = nullptr;
IScalarSensor<SensorValue<float>> *frequency = nullptr;
IScalarSensor<SensorValue<float>> *powerFactor = nullptr;
IScalarSensor<SensorValue<float>> *voltage = nullptr;

void sensors_sys_init() {
    activePower = getActivePowerSensor();
    reactivePower = getReactivePowerSensor();
    dailyKwh = getDailyKwhSensor();
    current = getCurrentSensor();
    frequency = getLineFrequencySensor();
    powerFactor = getPowerFactorSensor();
    voltage = getVoltageSensor();
}
