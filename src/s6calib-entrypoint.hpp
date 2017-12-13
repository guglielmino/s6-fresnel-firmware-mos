/*
 * Calibration firmware entry point
 */

#include "mgos.h"

#include "consts.h"
#include "globals.h"
#include "devfunctions.h"
#include "hardware/devices/MCP39F511/MCP39F511Consts.h"

#include "./setup/sensors_sys.hpp"


void printValues() {
    LOG(LL_DEBUG, ("VALUES"));
    // Trash first read
    reactivePower->readValue();

    SensorValue<float> reactiveValue = reactivePower->readValue();
    if(reactiveValue.isValid()) {
        LOG(LL_DEBUG, ("Reactive power %.2f VA\n", reactiveValue.value()));
    }

    SensorValue<float> powerValue = activePower->readValue();
    if(powerValue.isValid()) {
        LOG(LL_DEBUG, ("Active power %.2f W\n", powerValue.value()));
    }

    SensorValue<float> currentValue = current->readValue();
    if(currentValue.isValid()) {
        LOG(LL_DEBUG, ("Current %.2f A\n", currentValue.value()));
    }

    SensorValue<float> freqValue = frequency->readValue();
    if(freqValue.isValid()) {
        LOG(LL_DEBUG, ("Frequency %.2f Hz\n", freqValue.value()));
    }

    SensorValue<float> powerFactorValue = powerFactor->readValue();
    if(powerFactorValue.isValid()) {
        LOG(LL_DEBUG, ("Power factor %.2f \n", powerFactorValue.value()));
    }

    SensorValue<float> voltageValue = voltage->readValue();
    if(voltageValue.isValid()) {
        LOG(LL_DEBUG, ("Voltage  %.2f V\n", voltageValue.value()));
    }
}

enum mgos_app_init_result mgos_app_init(void) {
    IScalarSensor<SensorValue<float>> *targetCurrent = getReadU32Register(MCP_REG_CALIB_CURRENT, 10000.0);
    IScalarSensor<SensorValue<float>> *targetVoltage = getReadU16Register(MCP_REG_CALIB_VOLTAGE, 10.0);
    IScalarSensor<SensorValue<float>> *targetActivePower = getReadU32Register(MCP_REG_CALIB_POW_A, 100.0);
    IScalarSensor<SensorValue<float>> *targetReactivePower = getReadU32Register(MCP_REG_CALIB_POW_R, 100.0);

    LOG(LL_DEBUG, ("CALIBRATION FIRMWARE"));

    sensors_sys_init();

    printValues();

    LOG(LL_DEBUG, ("TARGET VALUES"));
    SensorValue<float> tCurrent = targetCurrent->readValue();
    if(tCurrent.isValid()) {
        LOG(LL_DEBUG, ("Target current %.2f A\n", tCurrent.value()));
    }

    SensorValue<float> tVoltage = targetVoltage->readValue();
    if(tVoltage.isValid()) {
        LOG(LL_DEBUG, ("Target voltage %.2f V\n", tVoltage.value()));
    }

    SensorValue<float> tAPower = targetActivePower->readValue();
    if(tAPower.isValid()) {
        LOG(LL_DEBUG, ("Target active power %.2f W\n", tAPower.value()));
    }

    SensorValue<float> tRPower = targetReactivePower->readValue();
    if(tRPower.isValid()) {
        LOG(LL_DEBUG, ("Target active power %.2f \n", tRPower.value()));
    }


    return MGOS_APP_INIT_SUCCESS;
}

