#pragma once

#include "../hardware/io/IUART.h"
#include "../hardware/io/UARTInterface.hpp"
#include "../hardware/devices/MCP39F511/uart/IMCP39F511UARTProto.h"
#include "../hardware/devices/MCP39F511/uart/MCP39F511UARTProto.hpp"

IUART *_uart = nullptr;
IMCP39F511UARTProto *_mcpProtoManager = nullptr;

IUART *setupUART() {
    if (_uart == nullptr) {
        UARTConfig config;
        config.baud_rate = 115200;
        config.num_data_bits = 8;
        config.parity = UART_PARITY_NONE;
        config.stop_bits = UART_STOP_BITS_1;

        //tx = 36, rx = 4
        _uart = dynamic_cast<IUART *>(new UARTInterface(2, config, 36, 4));
    }
    return _uart;
}

IMCP39F511UARTProto *setupMCPProto() {
    if (_mcpProtoManager == nullptr) {
        _mcpProtoManager = new MCP39F511UARTProto(setupUART());
    }
    return _mcpProtoManager;
}




