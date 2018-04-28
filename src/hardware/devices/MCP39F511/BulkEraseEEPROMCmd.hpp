//
// Created by Fabrizio Guglielmino on 29/10/17.
//


#ifndef __BULKERASE_H
#define __BULKERASE_H

#include "MCP39F511Command.hpp"

class BulkEraseEEPROMCmd : public MCP39F511Command{
public:
    BulkEraseEEPROMCmd()  {

    }

    std::vector<uint8_t> make() {
        std::vector<uint8_t> cmd;
        cmd.push_back(CMD_ERASE_EEPROM);

        return cmd;
    }
};

#endif

