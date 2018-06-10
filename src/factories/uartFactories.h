#pragma once

#include "../consts.h"
#include "../hardware/io/IUART.h"
#include "../hardware/io/UARTInterface.hpp"
#include "../hardware/devices/MCP39F511/uart/IMCP39F511UARTProto.h"
#include "../hardware/devices/MCP39F511/uart/MCP39F511UARTProto.hpp"

IUART *_uart = nullptr;

IUART *setupUART() {
    if (_uart == nullptr) {
        UARTConfig config;
        config.baud_rate = S6_UART_BAUDRATE;
        config.num_data_bits = 8;
        config.parity = UART_PARITY_NONE;
        config.stop_bits = UART_STOP_BITS_1;

        //tx = 36, rx = 4
        _uart = dynamic_cast<IUART *>(new UARTInterface(2, config, S6_UART_TX, S6_UART_RX));
    }
    return _uart;
}

IMCP39F511UARTProto *_mcpProtoManager = nullptr;

IMCP39F511UARTProto *setupMCPProto() {
    if (_mcpProtoManager == nullptr) {
        _mcpProtoManager = new MCP39F511UARTProto(setupUART());
    }
    return _mcpProtoManager;
}




