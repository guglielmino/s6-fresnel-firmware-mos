//
// Created by Fabrizio Guglielmino on 11/11/17.
//

#pragma once

#include "hardware/devices/MCP39F511/MCP39F511Utils.hpp"

#include "interfaces/ISensorCommand.hpp"
#include "interfaces/IUART.h"

/**
 * Start Energy accumulation
 */
class S6MCP39F51StartDailyKwh : public ISensorCommand, MCP39F511Utils {
private:
    IUART *_uart;

public:
    S6MCP39F51StartDailyKwh(IUART *uart)  : _uart(uart) {

    }

    void exec() {
        writeRegister(_uart, MCP_REG_RST_ENERGY_CNT, { 0x80 });
    }
};