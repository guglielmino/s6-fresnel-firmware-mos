//
// Created by Fabrizio Guglielmino on 29/10/17.
//

#ifndef __AUTOCALIBRATEFREQ_H
#define __AUTOCALIBRATEFREQ_H

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

#endif