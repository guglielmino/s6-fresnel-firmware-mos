//
// Created by Fabrizio Guglielmino on 01/10/17.
//

#pragma once


#include "../IADCReader.h"
#include "../IScalarSensor.h"
#include "../hardware/devices/xsns_hlw8012.h"

class SONOFFPowerSensor : public IScalarSensor<float> {
public:
    SONOFFPowerSensor() {

    }

    float readValue() {
        float ped, pi, pc;
        uint16_t pe, pw, pu;
        hlw_readEnergy(1, ped, pe, pw, pu, pi, pc);
        return pw;
    }

};