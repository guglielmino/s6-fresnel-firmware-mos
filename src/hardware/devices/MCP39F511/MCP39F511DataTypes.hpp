//
// Created by Fabrizio Guglielmino on 29/11/17.
//

#pragma once

class MCP39F511DataTypes {
public:
    int16_t u16(const uint8_t *msg_buffer, uint8_t addr){
        uint16_t offset = 2;
        return ((msg_buffer[addr + offset + 1] << 8) + msg_buffer[addr + offset + 0]);
    }

    void w_u16(uint16_t value, uint8_t *buffer) {
        buffer[0] = value & 0x00FF;
        buffer[1] = (value & 0xFF00) >> 8;
    }


    uint32_t u32(const uint8_t *msg_buffer, uint8_t addr){
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

    void w_u32(uint32_t value, uint8_t *buffer) {
        buffer[0] = value & 0x00FF;
        buffer[1] = (value & 0xFF00) >> 8;
        buffer[2] = (value & 0xFF0000) >> 16;
        buffer[3] = (value & 0xFF000000) >> 24;
    }

    uint64_t u64(const uint8_t *msg_buffer, uint8_t addr){
        uint64_t tmp = 0;
        uint64_t ret = 0;
        uint16_t offset = 2;
        tmp += msg_buffer[addr + 7 + offset];
        tmp = tmp << (7 * 8);
        for (uint8_t i = 7; i > 0; --i) {

            tmp = msg_buffer[addr + i + offset];
            tmp = tmp << (i * 8);
            ret += tmp;
        }

        ret += msg_buffer[addr + 0 + offset];

        return ret;
    }

};