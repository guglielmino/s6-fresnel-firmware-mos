//
// Created by Fabrizio Guglielmino on 29/10/17.
//

#pragma once

#include "MCP39F511Command.hpp"


class RegisterReadCmd : public MCP39F511Command{
private:
    uint8_t _numBytes;

public:
    RegisterReadCmd(uint8_t numBytes) : _numBytes(numBytes) {

    }

    std::vector<uint8_t> make() {
        std::vector<uint8_t> cmd;
        cmd.push_back(CMD_REGISTER_READ);
        cmd.push_back(_numBytes);
        return cmd;
    }
};
