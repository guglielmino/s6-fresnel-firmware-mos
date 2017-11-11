//
// Created by Fabrizio Guglielmino on 29/10/17.
//

#pragma once

#include <vector>
#include "MCP39F511Command.hpp"
#include "AddressPointerCmd.hpp"
#include "RegisterReadCmd.hpp"
#include "RegisterWriteCmd.hpp"

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
        tmp += msg_buffer[addr + 7 + offset];
        tmp = tmp << (7  * 8);
        for (uint8_t i = 6; i > 0; --i) {

            tmp = msg_buffer[addr + i + offset];
            tmp = tmp << (i  * 8);
            ret += tmp;
        }

        ret += msg_buffer[addr+0 + offset];

        return ret;
    }

    bool checkResp(char *readbuf) {
        bool ret = false;
        if (readbuf[0] == 0x06) {
            LOG(LL_DEBUG, ("*** Values Packet - Passed"));
            ret = true;
        }

        else if (readbuf[0] == 0x15) {
            LOG(LL_DEBUG, ("*** Values Packet - Failed"));
        }

        else if (readbuf[0] == 0x51) {
            LOG(LL_DEBUG, ("*** Packet - CRC Failed"));
        }
        else {
            LOG(LL_DEBUG, ("*** Packet - UNKNOW 0x%x", readbuf[0]));
        }

        return ret;
    }

    bool readRegister(IUART *uart, uint16_t regAddress, uint8_t size, char *buffer, size_t len) {
        AddressPointerCmd setPointer((regAddress & 0xff00) >> 8, regAddress & 0x00ff);
        RegisterReadCmd readRegister(size);

        std::vector<MCP39F511Command *> commands;
        commands.push_back(&setPointer);
        commands.push_back(&readRegister);

        std::vector<uint8_t> frame = makeFrame(commands);

        writeFrame(uart, frame);

        mgos_usleep(5000);

        size_t read = uart->read(buffer, len);
        return (read > 0 && checkResp(buffer));
    }

    bool writeRegister(IUART *uart, uint16_t regAddress, std::vector<uint8_t> data) {
        AddressPointerCmd setPointer((regAddress & 0xff00) >> 8, regAddress & 0x00ff);
        RegisterWriteCmd writeRegister(data);

        std::vector<MCP39F511Command *> commands;
        commands.push_back(&setPointer);
        commands.push_back(&writeRegister);

        std::vector<uint8_t> frame = makeFrame(commands);

        writeFrame(uart, frame);

        mgos_usleep(5000);

        char buffer[MCP_MAX_BUF_LEN];

        size_t read = uart->read(buffer, MCP_MAX_BUF_LEN);

        return (read > 0 && checkResp(buffer));
    }

private:

    void writeFrame(IUART *uart, std::vector<uint8_t> frame) {
        int frameSize = frame.size()  ;

        for (unsigned int i = 0; i < frameSize; ++i) {
            uart->write(&frame[i], 1);
            uart->flush();
            mgos_usleep(1000);
        }
    }


};