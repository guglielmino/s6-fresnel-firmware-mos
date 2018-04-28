//
// Created by Fabrizio Guglielmino on 29/10/17.
//

#ifndef __AUTOCALIBRATEGAIN_H
#define __AUTOCALIBRATEGAIN_H

#include "MCP39F511Command.hpp"

class AutoCalibrateGainCmd : public MCP39F511Command{
public:
    AutoCalibrateGainCmd() {

    }

    std::vector<uint8_t> make() {
        std::vector<uint8_t> cmd;
        cmd.push_back(CMD_AUTO_CALIBRATE_GAIN);

        return cmd;
    }
};

#endif
