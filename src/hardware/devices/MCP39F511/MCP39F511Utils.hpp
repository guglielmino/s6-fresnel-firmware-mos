//
// Created by Fabrizio Guglielmino on 29/10/17.
//

#pragma once

#include <vector>
#include "MCP39F511Command.hpp"
#include "AddressPointerCmd.hpp"
#include "RegisterReadCmd.hpp"
#include "RegisterWriteCmd.hpp"
#include "MCP39F511DataTypes.hpp"



#define READ_AVAIL_TIMEOUT_MS 700 // Timeout for checking read availability (ms)

class MCP39F511Utils {
protected:
    MCP39F511DataTypes dataTypes;

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

        uint32_t start = millis();
        size_t avail = 0;
        while (avail == 0 && (millis() - start) < READ_AVAIL_TIMEOUT_MS) {
            avail = uart->readAvail();
        }

        uint32_t buffsize = 2 + size + 1; // ACK + Num bytes + bytes + checksum
        size_t read = uart->read(buffer, buffsize);

        return (read > 0 && checkResp(buffer));
    }

    void writeRegister(IUART *uart, uint16_t regAddress, std::vector<uint8_t> data) {
        AddressPointerCmd setPointer((regAddress & 0xff00) >> 8, regAddress & 0x00ff);
        RegisterWriteCmd writeRegister(data);

        std::vector<MCP39F511Command *> commands;
        commands.push_back(&setPointer);
        commands.push_back(&writeRegister);

        std::vector<uint8_t> frame = makeFrame(commands);

        writeFrame(uart, frame);
    }

    void execCommand(IUART *uart, MCP39F511Command& command) {
        std::vector<MCP39F511Command *> commands;
        commands.push_back(&command);

        std::vector<uint8_t> frame = makeFrame(commands);

        writeFrame(uart, frame);
    }

private:
    unsigned long millis(void) {
        return mgos_uptime() * 1000;
    }


    void writeFrame(IUART *uart, std::vector<uint8_t> frame) {
        int frameSize = frame.size()  ;

        for (unsigned int i = 0; i < frameSize; ++i) {
            uart->write(&frame[i], 1);
            uart->flush();
            mgos_usleep(1000);
        }
    }


};