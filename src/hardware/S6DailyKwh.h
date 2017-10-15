//
// Created by Fabrizio Guglielmino on 01/10/17.
//

#pragma once

#pragma once

#include "../interfaces/IADCReader.h"
#include "../interfaces/IScalarSensor.h"


class S6DailyKwh : public IScalarSensor<unsigned long> {
public:
    S6DailyKwh() {

    }

    unsigned long readValue() {
        return 0;
    }
};