//
// Created by Fabrizio Guglielmino on 01/10/17.
//

#pragma once

#include "interfaces/IADCReader.h"
#include "interfaces/IScalarSensor.h"
#include "hardware/devices/xsns_hlw8012.h"
#include "SensorValue.hpp"

class SONOFFDailyKwh : public IScalarSensor<SensorValue<float>> {
public:
    SONOFFDailyKwh() {
        hlw_init();
    }

    SensorValue<float> readValue() {
        SensorValue<float> ret(0, false);

        return ret;
    }
};