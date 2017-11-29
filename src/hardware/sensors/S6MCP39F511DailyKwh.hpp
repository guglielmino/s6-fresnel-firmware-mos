//
// Created by Fabrizio Guglielmino on 01/10/17.
//

#pragma once

#include "hardware/devices/MCP39F511/MCP39F511Utils.hpp"

#include "interfaces/IADCReader.h"
#include "interfaces/IScalarSensor.h"

class S6MCP39F511DailyKwh : public IScalarSensor<float>, MCP39F511Utils {
private:
    IUART *_uart;
    static const int  BUFFER_SIZE = 12;

public:
    S6MCP39F511DailyKwh(IUART *uart) : _uart(uart) {

    }

    float readValue() {
        float ret = 0;
        char buffer[S6MCP39F511DailyKwh::BUFFER_SIZE];
        bool success = readRegister(_uart, MCP_REG_IMP_ACTIVE_CNT, 8, buffer, S6MCP39F511DailyKwh::BUFFER_SIZE);

        if (success) {
            uint64_t Kwh = dataTypes.u64(buffer, 0);
            ret = (Kwh / 1000000.0);
        } else {
            LOG(LL_DEBUG, ("*** READ KWH FAILED!!!"));
        }

        return ret;
    }
};