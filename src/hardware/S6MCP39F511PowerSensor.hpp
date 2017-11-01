//
// Created by Fabrizio Guglielmino on 10/10/17.
//

#pragma once

#include "../hardware/devices/MCP39F511/MCP39F511Utils.hpp"

#include "../interfaces/IScalarSensor.h"
#include "../interfaces/IUART.h"


#define POWER_RESP_BUFFER_SIZE 7

class S6MCP39F511PowerSensor : public IScalarSensor<float>, MCP39F511Utils {
private:
    IUART *_uart;

public:
    S6MCP39F511PowerSensor(IUART *uart)  : _uart(uart) {

    }

    float readValue() {
        float ret = 0.0;
        char buffer[POWER_RESP_BUFFER_SIZE];
        bool success = readRegister(_uart, MCP_REG_ACTIVE_POWER, 4, buffer, POWER_RESP_BUFFER_SIZE);

        if(success) {
            uint32_t power = u32(buffer, 0);
            ret = (power / 100.0);
        }

        return ret;
    }
};


