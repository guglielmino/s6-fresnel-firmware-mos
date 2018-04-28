//
// Created by Fabrizio Guglielmino on 29/10/17.
//

#ifndef __MCP39F511UTILS_H
#define __MCP39F511UTILS_H

#include <vector>
#include "MCP39F511Command.hpp"
#include "AddressPointerCmd.hpp"
#include "RegisterReadCmd.hpp"
#include "RegisterWriteCmd.hpp"


std::vector<uint8_t> prepareReadFrame(uint16_t regAddress, uint8_t size) {
    AddressPointerCmd setPointer((regAddress & 0xff00) >> 8, regAddress & 0x00ff);
    RegisterReadCmd readRegister(size);

    std::vector<MCP39F511Command *> commands;
    commands.push_back(&setPointer);
    commands.push_back(&readRegister);

    return makeFrame(commands);
}

std::vector<uint8_t>  prepareWriteFrame(uint16_t regAddress, std::vector<uint8_t> data) {
    AddressPointerCmd setPointer((regAddress & 0xff00) >> 8, regAddress & 0x00ff);
    RegisterWriteCmd writeRegister(data);

    std::vector < MCP39F511Command *> commands;
    commands.push_back(&setPointer);
    commands.push_back(&writeRegister);

    return makeFrame(commands);
}

#endif