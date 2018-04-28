//
// Created by Fabrizio Guglielmino on 29/10/17.
//

#ifndef __SAVEREGTOFLASH_H
#define __SAVEREGTOFLASH_H

#include "MCP39F511Command.hpp"

class SaveRegistersToFlashCmd : public MCP39F511Command{
public:
    SaveRegistersToFlashCmd()  {

    }

    std::vector<uint8_t> make() {
        std::vector<uint8_t> cmd;
        cmd.push_back(CMD_SAVE_TO_FLASH);

        return cmd;
    }
};

#endif
