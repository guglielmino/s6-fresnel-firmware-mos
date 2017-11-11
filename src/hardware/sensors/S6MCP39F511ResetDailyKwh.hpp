//
// Created by Fabrizio Guglielmino on 11/11/17.
//

#pragma once

#include "hardware/devices/MCP39F511/MCP39F511Utils.hpp"

#include "interfaces/ISensorCommand.hpp"
#include "interfaces/IUART.h"


#define POWER_RESP_BUFFER_SIZE 7

/**
 * Reset and stop Energy accumulation
 */
class S6MCP39F511ResetDailyKwh : public ISensorCommand, MCP39F511Utils {
private:
    IUART *_uart;

public:
    S6MCP39F511ResetDailyKwh(IUART *uart)  : _uart(uart) {

    }

    bool exec() {
        return writeRegister(_uart, MCP_REG_RST_ENERGY_CNT, { 0x00 });
    }
};