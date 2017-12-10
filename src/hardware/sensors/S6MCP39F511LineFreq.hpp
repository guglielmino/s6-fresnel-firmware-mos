//
// Created by Fabrizio Guglielmino on 11/11/17.
//

#pragma once

#include "hardware/devices/MCP39F511/MCP39F511Utils.hpp"

#include "interfaces/IADCReader.h"
#include "interfaces/IScalarSensor.h"
#include "SensorValue.hpp"

class S6MCP39F511LineFreq : public IScalarSensor<SensorValue<float>>, MCP39F511Utils {
private:
    IUART *_uart;
    static const int BUFFER_SIZE = 5;

public:
    S6MCP39F511LineFreq(IUART *uart) : _uart(uart) {

    }

    SensorValue<float> readValue() {
        SensorValue<float> ret(0, false);
        char buffer[S6MCP39F511LineFreq::BUFFER_SIZE];

        bool success = readRegister(_uart, MCP_REG_FREQUENCY, 2, buffer, S6MCP39F511LineFreq::BUFFER_SIZE);

        if (success) {
            uint16_t Freq = dataTypes.u16(buffer, 0);
            ret.setValue((Freq / 1000.0));
        } else {
            LOG(LL_DEBUG, ("*** READ FREQUENCY FAILED!!!"));
        }

        return ret;
    }
};