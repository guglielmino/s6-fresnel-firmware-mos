//
// Created by Fabrizio Guglielmino on 01/10/17.
//

#pragma once


#include "interfaces/IADCReader.h"
#include "interfaces/IScalarSensor.h"
#include "hardware/devices/xsns_hlw8012.h"
#include "SensorValue.hpp"

class SONOFFPowerSensor : public IScalarSensor<SensorValue<float>> {
public:
    SONOFFPowerSensor() {
        hlw_init();
    }

    float readValue() {
        SensorValue<float> ret(0, false);

        return ret;
    }

};