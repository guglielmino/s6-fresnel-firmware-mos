//
// Created by Fabrizio Guglielmino on 29/10/17.
//

#pragma once

#include "MCP39F511Command.hpp"

class RegisterWriteCmd : public MCP39F511Command{
private:
    std::vector<uint8_t> _data;

public:
    RegisterWriteCmd(std::vector<uint8_t> &data) : _data(data) {

    }

    std::vector<uint8_t> make() {
        std::vector<uint8_t> cmd;
        cmd.push_back(CMD_REGISTER_WRITE);
        cmd.push_back(_data.size());
        cmd.insert(cmd.end(), _data.begin(), _data.end());
        return cmd;
    }
};
