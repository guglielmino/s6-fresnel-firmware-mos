#pragma once

#pragma once

#include <vector>

#define ACK 0x06
#define NACK 0x15
#define CSFAIL 0x51

#define MCP_MAX_BUF_LEN 40

#define COMMAND_HEADER_BYTE 0xA5

// Commands
#define CMD_REGISTER_READ 0x4E
#define CMD_REGISTER_WRITE 0x4D
#define CMD_SET_ADDRESS_POINTER 0x41
#define CMD_SAVE_TO_FLASH 0x53
#define CMD_READ_EEPROM 0x42
#define CMD_WRITE_EEPROM 0x50
#define CMD_ERASE_EEPROM 0x4F
#define CMD_AUTO_CALIBRATE_GAIN 0x5A
#define CMD_AUTO_CALIBRATE_Q_GAIN 0x7A
#define CMD_AUTO_CALIBRATE_F 0x76

// Registers
#define MCP_REG_INST_POINTER    0x0000 // Instruction Pointer 6.2 R u16 Address pointer for read or write commands
#define MCP_REG_SYSTEM_STATUS   0x0002 // System Status 6.3 R b16 System Status Register
#define MCP_REG_SYSTEM_VERSION  0x0004 // System Version 6.4 R u16 System version date code information for MCP39F511N, set at the Microchip factory; format YMDD
#define MCP_REG_VOLTS           0x0006 // Voltage RMS 5.4 R u16 RMS Voltage output
#define MCP_REG_FREQUENCY       0x0008 // Line Frequency 5.1.1 R u16 Line Frequency output
#define MCP_REG_ANALINPUT_V     0x000A // Analog input vltage
#define MCP_REG_POWER_FACTOR    0x000C // Power factor
#define MCP_REG_CURRENT_RMS     0x000E // Current RMS
#define MCP_REG_ACTIVE_POWER    0x0012 // Active power
#define MCP_REG_REACTIVE_POWER  0x0016 // Reactive power
#define MCP_REG_APPARENT_POWER  0x001A // Apparent power
#define MCP_REG_IMP_ACTIVE_CNT  0x001E // Import Active Energy Counter
#define MCP_REG_EXP_ACTIVE_CNT  0x0026 // Export Active Energy Counter
#define MCP_REG_IMP_REACT_CNT   0x002E // Import Reactive Energy Counter
#define MCP_REG_EXP_REACT_CNT   0x0036 // Export Reactive Energy Counter
#define MCP_REG_RST_ENERGY_CNT  0x00DC // reset/start Energy accumulation


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