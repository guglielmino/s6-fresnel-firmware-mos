//
// Created by Fabrizio Guglielmino on 29/10/17.
//

#pragma once

class MCP39F511Utils {
protected:
    int16_t readInt(char * msg_buffer, uint8_t addr){
        uint16_t offset = 2;
        return ((msg_buffer[addr + offset + 1] << 8) + msg_buffer[addr + offset + 0]);
    }

    uint32_t readLong(char * msg_buffer, uint8_t addr){
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
};