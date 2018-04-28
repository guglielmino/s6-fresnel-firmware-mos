#ifndef __MCP39F511COMMAND_H
#define __MCP39F511COMMAND_H

#include <vector>
#include "MCP39F511Consts.h"


uint8_t checksum(const uint8_t *buffer, size_t len) {
    uint32_t cs = 0;
    uint32_t tmp = 0;

    for(uint8_t i=0; i< len; i++){
        tmp = buffer[i];
        cs = cs + tmp;
    }

    return (cs % 256) & 0xFF;
}

class MCP39F511Command {
public:
    virtual std::vector<uint8_t> make() = 0;
};

std::vector<uint8_t> makeFrame(std::vector<MCP39F511Command *> commands) {
    std::vector <uint8_t> frame;

    std::vector<uint8_t> commandsbuffer;
    uint8_t commandSize = 0;
    for (std::vector<MCP39F511Command *>::iterator it = commands.begin() ; it != commands.end(); ++it) {
        MCP39F511Command *cmd = (*it);
        std::vector<uint8_t> cmdbuffer = cmd->make();
        commandsbuffer.insert(commandsbuffer.end(), cmdbuffer.begin(), cmdbuffer.end());
        commandSize += cmdbuffer.size();
    }

    uint8_t frameSize = (2 + commandSize + 1);

    // Header and size
    frame.push_back(COMMAND_HEADER_BYTE);
    frame.push_back((uint8_t)frameSize);
    // Add commands
    frame.insert(frame.end(), commandsbuffer.begin(), commandsbuffer.end());
    // Checksum
    uint8_t chksum = checksum(frame.data(), frame.size());
    frame.push_back(chksum);

    return frame;
}

#endif