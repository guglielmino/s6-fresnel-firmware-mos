//
// Created by Fabrizio Guglielmino on 29/10/17.
//

#pragma once

#include <vector>
#include "MCP39F511Command.hpp"
#include "AddressPointerCmd.hpp"
#include "RegisterReadCmd.hpp"

class MCP39F511Utils {
protected:
    int16_t u16(char *msg_buffer, uint8_t addr){
        uint16_t offset = 2;
        return ((msg_buffer[addr + offset + 1] << 8) + msg_buffer[addr + offset + 0]);
    }

    uint32_t u32(char *msg_buffer, uint8_t addr){
        uint32_t tmp = 0;
        uint32_t ret = 0;
        uint16_t offset = 2;
        tmp += msg_buffer[addr+3 + offset];
        tmp = tmp << 24;
        ret += tmp;
        tmp = msg_buffer[addr+2 + offset];
        tmp = tmp << 16;
        ret += tmp;
        tmp = msg_buffer[addr+1 + offset];
        tmp = tmp << 8;
        ret += tmp;
        ret += msg_buffer[addr+0 + offset];
        return ret;
    }

    uint64_t u64(char *msg_buffer, uint8_t addr){
        uint64_t tmp = 0;
        uint64_t ret = 0;
        uint16_t offset = 2;
        for (uint8_t i = sizeof(uint64_t) / 8; i > 0; --i) {
            tmp += msg_buffer[addr + i + offset];
            tmp = tmp << (i * 8);
            ret += tmp;
        }

        ret += msg_buffer[addr+0 + offset];
        return ret;
    }

    bool checkResp(char *readbuf) {
        bool ret = false;
        if (readbuf[0] == 0x06) {
            LOG(LL_DEBUG, ("Values Packet - Passed"));
            ret = true;
        }

        else if (readbuf[0] == 0x15) {
            LOG(LL_DEBUG, ("Values Packet - Failed"));
        }

        else if (readbuf[0] == 0x51) {
            LOG(LL_DEBUG, ("Packet - CRC Failed"));
        }

        return ret;
    }

    bool readRegister(IUART *uart, uint16_t regAddress, uint8_t size, char *buffer, size_t len) {
        float ret = 0.0;
        AddressPointerCmd setPointer((regAddress & 0xff00) >> 8, regAddress & 0x00ff);
        RegisterReadCmd readRegister(size);

        std::vector<MCP39F511Command *> commands;
        commands.push_back(&setPointer);
        commands.push_back(&readRegister);

        std::vector<uint8_t> frame = makeFrame(commands);
        int frameSize = frame.size()  ;

        for (unsigned int i = 0; i < frameSize; ++i) {
            uart->write(&frame[i], 1);
            uart->flush();
            mgos_usleep(1000);
        }
        mgos_usleep(5000);

        size_t read = uart->read(buffer, len);
        return (read > 0 && checkResp(buffer));
    }
};