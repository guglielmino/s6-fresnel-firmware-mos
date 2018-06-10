//
// Created by Fabrizio Guglielmino on 10/06/18.
//

#pragma once

#include "../hardware/devices/MCP39F511/uart/IMCP39F511UARTProto.h"

IMCP39F511UARTProto *mcp39F511UARTProto = nullptr;

void ext_device_sys_init() {
    mcp39F511UARTProto = setupMCPProto();
}