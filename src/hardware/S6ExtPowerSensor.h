//
// Created by Fabrizio Guglielmino on 10/10/17.
//

#pragma once

#include "../interfaces/IScalarSensor.h"


class S6ExtPowerSensor : public IScalarSensor<float> {
private:

public:
    S6ExtPowerSensor()  {

    }

    float readValue() {

        return 0;
    }
};


