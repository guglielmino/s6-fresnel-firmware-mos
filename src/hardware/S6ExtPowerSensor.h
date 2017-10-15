//
// Created by Fabrizio Guglielmino on 10/10/17.
//

#pragma once

#include "mgos_spi.h"
#include "../interfaces/IScalarSensor.h"
#include "../interfaces/ISPI.h"

/*
 * Sample there https://github.com/mongoose-os-apps/example-spi-c/blob/master/src/main.c
 */
class S6ExtPowerSensor : public IScalarSensor<float> {
private:

    ISPI *_spi = nullptr;

public:
    S6ExtPowerSensor(ISPI *spi) : _spi(spi) {

    }

    float readValue() {
        return 0;
    }
};


