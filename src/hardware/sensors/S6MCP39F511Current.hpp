//
// Created by Fabrizio Guglielmino on 11/11/17.
//

#pragma once

#include "hardware/devices/MCP39F511/MCP39F511Utils.hpp"

#include "interfaces/IADCReader.h"
#include "interfaces/IScalarSensor.h"

class S6MCP39F511Current : public IScalarSensor<float>, MCP39F511Utils {
private:
    IUART *_uart;
    static const int BUFFER_SIZE = 7;

public:
    S6MCP39F511Current(IUART *uart) : _uart(uart) {

    }

    float readValue() {
        float ret = 0;
        char buffer[S6MCP39F511Current::BUFFER_SIZE];

        bool success = readRegister(_uart, MCP_REG_CURRENT_RMS, 4, buffer, S6MCP39F511Current::BUFFER_SIZE);

        if (success) {
            uint32_t Current = dataTypes.u32(buffer, 0);
            ret = (Current / 10000.0);
        } else {
            LOG(LL_DEBUG, ("*** READ CURRENT FAILED!!!"));
        }

        return ret;
    }
};