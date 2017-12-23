//
// Created by Fabrizio Guglielmino on 13/12/17.
//

#pragma once

#include <functional> 

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
#define MCP_REG_CALIB_CURRENT   0x0086 // Target Current to be used during single-point calibration
#define MCP_REG_CALIB_VOLTAGE   0x008A // Target Voltage to be used during single-point calibration
#define MCP_REG_CALIB_POW_A     0x008C // Target Active Power to be used during single-point calibration
#define MCP_REG_CALIB_POW_R     0x0090 // Target Reactive Power to be used during single-point calibration
#define MCP_REG_REF_FREQ        0x0094 // Reference Value for the nominal line frequency

template<typename T, typename R>  
struct McpRegister {
  uint16_t reg;
  uint8_t bufferSize;
  std::function<T(R)> rawToValue;
};

const static McpRegister<float, uint32_t> REG_CURRENT = { .reg = MCP_REG_CURRENT_RMS, .bufferSize = 7, .rawToValue = [](uint32_t value) { return (value / 10000.0); }};
const static McpRegister<float, uint64_t> REG_IMP_ACTIVE_CNT = { .reg = MCP_REG_IMP_ACTIVE_CNT, .bufferSize = 12, .rawToValue = [](uint64_t value) { return (value / 1000000.0); }};
const static McpRegister<float, uint16_t> REG_FREQUENCY = { .reg = MCP_REG_FREQUENCY, .bufferSize = 2, .rawToValue = [](uint16_t value) { return (value / 1000.0); }};
const static McpRegister<float, uint16_t> REG_POWER_FACTOR = { .reg = MCP_REG_POWER_FACTOR, .bufferSize = 2, .rawToValue = [](uint16_t value) { return (value * 0.000030517578125); }};
const static McpRegister<float, uint32_t> REG_ACTIVE_POWER = { .reg = MCP_REG_ACTIVE_POWER, .bufferSize = 7, .rawToValue = [](uint32_t value) { return (value / 100.0); }};
const static McpRegister<float, uint32_t> REG_REACTIVE_POWER = { .reg = MCP_REG_REACTIVE_POWER, .bufferSize = 7, .rawToValue = [](uint32_t value) { return (value / 100.0); }};
const static McpRegister<float, uint32_t> REG_APPARENT_POWER = { .reg = MCP_REG_APPARENT_POWER, .bufferSize = 7, .rawToValue = [](uint32_t value) { return (value / 100.0); }};
const static McpRegister<float, uint16_t> REG_VOLTAGE = { .reg = MCP_REG_VOLTS, .bufferSize = 5, .rawToValue = [](uint16_t value) { return (value / 10.0); }};

const static McpRegister<float, uint16_t> REG_CALIB_VOLTAGE = { .reg = MCP_REG_CALIB_VOLTAGE, .bufferSize = 5, .rawToValue = [](uint16_t value) { return (value / 10.0); }};
const static McpRegister<float, uint32_t> REG_CALIB_CURRENT = { .reg = MCP_REG_CALIB_CURRENT, .bufferSize = 7, .rawToValue = [](uint32_t value) { return (value / 10000.0); }};
const static McpRegister<float, uint32_t> REG_CALIB_POW_A = { .reg = MCP_REG_CALIB_POW_A, .bufferSize = 7, .rawToValue = [](uint32_t value) { return (value / 100.0); }};
const static McpRegister<float, uint32_t> REG_CALIB_POW_R = { .reg = MCP_REG_CALIB_POW_R, .bufferSize = 7, .rawToValue = [](uint32_t value) { return (value / 100.0); }};
const static McpRegister<float, uint16_t> REG_REF_FREQUENCY = { .reg = MCP_REG_REF_FREQ, .bufferSize = 5, .rawToValue = [](uint16_t value) { return (value / 1000.0); }};