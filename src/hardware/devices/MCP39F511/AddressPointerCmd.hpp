//
// Created by Fabrizio Guglielmino on 29/10/17.
//

#pragma once

#include "MCP39F511Command.hpp"

class AddressPointerCmd : public MCP39F511Command{
private:
    uint8_t _addressHigh;
    uint8_t _addressLow;

public:
    AddressPointerCmd(uint8_t addressHigh, uint8_t addressLow) : _addressHigh(addressHigh), _addressLow(addressLow) {

    }

    std::vector<uint8_t> make() {
        std::vector<uint8_t> cmd;
        cmd.push_back(CMD_SET_ADDRESS_POINTER);
        cmd.push_back(_addressHigh);
        cmd.push_back(_addressLow);
        return cmd;
    }
};
