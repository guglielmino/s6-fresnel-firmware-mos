/*
 * Calibration firmware entry point
 */

#include <string>
#include <map>

#include "mgos.h"

#include "consts.h"
#include "globals.h"
#include "devfunctions.h"
#include "hardware/devices/MCP39F511/MCP39F511Consts.h"

#include "./setup/sensors_sys.hpp"



void printValues() {
    LOG(LL_INFO, ("VALUES"));
  
    std::map<std::string, float> values = getValues();

    if(values[REACTIVE_POWER]) {
        LOG(LL_INFO, ("Reactive power %.2f VA", values[REACTIVE_POWER]));
    }

    if(values[ACTIVE_POWER]) {
        LOG(LL_INFO, ("Active power %.2f W", values[ACTIVE_POWER]));
    }

    if(values[CURRENT]) {
        LOG(LL_INFO, ("Current %.2f A", values[CURRENT]));
    }
    if(values[FREQ]) {
        LOG(LL_INFO, ("Frequency %.2f Hz", values[FREQ]));
    }

    if(values[POWER_FACTOR]) {
        LOG(LL_INFO, ("Power factor %.2f", values[POWER_FACTOR]));
    }

    if(values[VOLTAGE]) {
        LOG(LL_INFO, ("Voltage  %.2f V", values[VOLTAGE]));
    }
}

void configOverride() {
    mgos_sys_config_set_mqtt_enable(false);
    mgos_sys_config_set_wifi_sta_enable(false);
    mgos_sys_config_set_eth_enable(false);
    mgos_sys_config_set_bt_enable(false);
}

enum mgos_app_init_result mgos_app_init(void) {
    cs_log_set_level(LL_INFO);
    configOverride();

    IScalarSensor<SensorValue<float>> *targetCurrent = getReadU32Register(REG_CALIB_CURRENT);
    IScalarSensor<SensorValue<float>> *targetVoltage = getReadU16Register(REG_CALIB_VOLTAGE); 
    IScalarSensor<SensorValue<float>> *targetActivePower = getReadU32Register(REG_CALIB_POW_A);
    IScalarSensor<SensorValue<float>> *targetReactivePower = getReadU32Register(REG_CALIB_POW_R);
    IScalarSensor<SensorValue<float>> *targetFreq = getReadU16Register(REG_REF_FREQUENCY);
    sensors_sys_init();

    LOG(LL_INFO, ("***** CALIBRATION FIRMWARE ********"));
    // Wait 80ms to
    mgos_usleep(8000);
    LOG(LL_INFO, ("Use SmartSix.SetOffsets to set offset registers (Current, Active and Reactive power)"));



  
    printValues();
    mgos_wdt_feed();

    LOG(LL_INFO, ("TARGET VALUES"));
    for(int i =0; i < 30; i++) {
        SensorValue<float> tCurrent = targetCurrent->readValue();
        mgos_wdt_feed();
        if(tCurrent.isValid()) {
            LOG(LL_INFO, ("Target current %.2f A", tCurrent.value()));
        }

        SensorValue<float> tVoltage = targetVoltage->readValue();
        mgos_wdt_feed();
        if(tVoltage.isValid()) {
            LOG(LL_INFO, ("Target voltage %.2f V", tVoltage.value()));
        }

        SensorValue<float> tAPower = targetActivePower->readValue();
        mgos_wdt_feed();
        if(tAPower.isValid()) {
            LOG(LL_INFO, ("Target active power %.2f W", tAPower.value()));
        }

        SensorValue<float> tRPower = targetReactivePower->readValue();
        mgos_wdt_feed();
        if(tRPower.isValid()) {
            LOG(LL_INFO, ("Target reactive power %.2f VA", tRPower.value()));
        }

        SensorValue<float> tFreq = targetFreq->readValue();
        mgos_wdt_feed();
        if(tFreq.isValid()) {
            LOG(LL_INFO, ("Reference frequency %.2f Hz", tFreq.value()));
        }
    }

    return MGOS_APP_INIT_SUCCESS;
}

