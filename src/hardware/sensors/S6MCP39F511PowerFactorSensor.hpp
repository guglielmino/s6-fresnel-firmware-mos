//
// Created by Fabrizio Guglielmino on 12/11/17.
//


#pragma once

#include "hardware/devices/MCP39F511/MCP39F511Utils.hpp"

#include "interfaces/IADCReader.h"
#include "interfaces/IScalarSensor.h"
#include "SensorValue.hpp"

class S6MCP39F511PowerFactorSensor : public IScalarSensor<SensorValue<float>>, MCP39F511Utils {
private:
    IUART *_uart;
    static const int BUFFER_SIZE = 5;

public:
    S6MCP39F511PowerFactorSensor(IUART *uart) : _uart(uart) {

    }

    SensorValue<float> readValue() {
        SensorValue<float> ret(0, false);
        char buffer[S6MCP39F511PowerFactorSensor::BUFFER_SIZE];

        bool success = readRegister(_uart, MCP_REG_POWER_FACTOR, 2, buffer, S6MCP39F511PowerFactorSensor::BUFFER_SIZE);

        if (success) {
            uint16_t PowerFactor = dataTypes.u16(buffer, 0);
            ret.setValue((PowerFactor * 0.000030517578125));
        } else {
            LOG(LL_DEBUG, ("*** READ POWER FACTOR FAILED!!!"));
        }

        return ret;
    }
};