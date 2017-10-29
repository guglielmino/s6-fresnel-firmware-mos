//
// Created by Fabrizio Guglielmino on 29/10/17.
//

#pragma once

#include "MCP39F511Command.hpp"

class AutoCalibrateFrequencyCmd: public MCP39F511Command{
public:
    AutoCalibrateFrequencyCmd() {

    }

    std::vector<uint8_t> make() {
        std::vector<uint8_t> cmd;
        cmd.push_back(CMD_AUTO_CALIBRATE_F);

        return cmd;
    }
};
