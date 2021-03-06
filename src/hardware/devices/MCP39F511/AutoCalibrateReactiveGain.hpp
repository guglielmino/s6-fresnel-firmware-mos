//
// Created by Fabrizio Guglielmino on 29/10/17.
//

#ifndef __AUTOCALIBRATEREACTGAIN_H
#define __AUTOCALIBRATEREACTGAIN_H

#include "MCP39F511Command.hpp"

class AutoCalibrateReactiveGainCmd : public MCP39F511Command{
public:
    AutoCalibrateReactiveGainCmd() {

    }

    std::vector<uint8_t> make() {
        std::vector<uint8_t> cmd;
        cmd.push_back(CMD_AUTO_CALIBRATE_Q_GAIN);

        return cmd;
    }
};

#endif
