//
// Created by Fabrizio Guglielmino on 29/10/17.
//

#pragma once

#include "../hardware/devices/MCP39F511/MCP39F511Utils.hpp"
#include "../hardware/devices/MCP39F511/MCP39F511Command.hpp"
#include "../hardware/devices/MCP39F511/AddressPointerCmd.hpp"
#include "../hardware/devices/MCP39F511/RegisterReadCmd.hpp"

#include "../interfaces/IScalarSensor.h"
#include "../interfaces/IUART.h"

#define VOLTAGE_RESP_BUFFER_SIZE 5

class S6MCP39F511VoltageSensor : public IScalarSensor<float>, MCP39F511Utils {
private:
    IUART *_uart;

public:
    S6MCP39F511VoltageSensor(IUART *uart)  : _uart(uart) {

    }

    float readValue() {
        float ret = 0.0;
        char buffer[VOLTAGE_RESP_BUFFER_SIZE];
        bool success = readRegister(_uart, MCP_REG_VOLTS, 2, buffer, VOLTAGE_RESP_BUFFER_SIZE);

        if(success) {
            uint16_t Vrms = u16(buffer, 0);
            ret = (Vrms / 10.0);
        }

       return ret;
    }
};