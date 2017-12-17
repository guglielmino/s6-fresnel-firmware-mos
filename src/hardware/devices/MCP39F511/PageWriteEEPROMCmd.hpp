//
// Created by Fabrizio Guglielmino on 29/10/17.
//

#pragma once

#include "MCP39F511Command.hpp"

class PageWriteEEPROMCmd : public MCP39F511Command{
private:
    std::vector<uint8_t> _data;
    uint8_t _pageNum;

public:
    PageWriteEEPROMCmd(uint8_t pageNum, std::vector<uint8_t> &data) : _pageNum(pageNum), _data(data) {

    }

    std::vector<uint8_t> make() {
        std::vector<uint8_t> cmd;
        cmd.push_back(CMD_WRITE_EEPROM);
        cmd.push_back(_pageNum);
        cmd.insert(cmd.end(), _data.begin(), _data.end());
        return cmd;
    }
};
