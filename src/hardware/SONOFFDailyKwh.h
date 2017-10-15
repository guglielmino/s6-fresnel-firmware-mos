//
// Created by Fabrizio Guglielmino on 01/10/17.
//

#pragma once

#include "../interfaces/IADCReader.h"
#include "../interfaces/IScalarSensor.h"
#include "../hardware/devices/xsns_hlw8012.h"

class SONOFFDailyKwh : public IScalarSensor<unsigned long> {
public:
    SONOFFDailyKwh() {
        hlw_init();
    }

    unsigned long readValue() {
        float ped, pi, pc;
        uint16_t pe, pw, pu;
        hlw_readEnergy(0, ped, pe, pw, pu, pi, pc);
        return (unsigned long)ped;
    }
};