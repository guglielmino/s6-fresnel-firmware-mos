//
// Created by Fabrizio Guglielmino on 29/10/17.
//

#ifndef __PAGEREADEEPROM_H
#define __PAGEREADEEPROM_H

#include "MCP39F511Command.hpp"

class PageReadEEPROMCmd : public MCP39F511Command{
private:
    uint8_t _pageNum;

public:
    PageReadEEPROMCmd(uint8_t pageNum) : _pageNum(pageNum) {

    }

    std::vector<uint8_t> make() {
        std::vector<uint8_t> cmd;
        cmd.push_back(CMD_READ_EEPROM);
        cmd.push_back(_pageNum);
        return cmd;
    }
};

#endif