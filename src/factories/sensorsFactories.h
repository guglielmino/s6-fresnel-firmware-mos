#pragma once

#include "../interfaces/IScalarSensor.h"
#include "../hardware/sensors/SensorValue.hpp"
#include "../interfaces/IADCReader.h"

#if CS_PLATFORM == CS_P_ESP8266
    #include "../hardware/sensors/SONOFFPowerSensor.h"
    #include "../hardware/sensors/SONOFFDailyKwh.h"

    IScalarSensor<SensorValue<float>> *getActivePowerSensor() {
        return new SONOFFPowerSensor();
    }

    IScalarSensor<SensorValue<float>> *getReactivePowerSensor() {
        #error "Not implemented in SONOFF device";
    }

    IScalarSensor<SensorValue<float>> *getVoltageSensor() {
        #error "No Voltage Sensor Implemented for SONOFF device";
    }

    IScalarSensor<SensorValue<float>> *getDailyKwhSensor() {
        return new SONOFFDailyKwh();
    }

    ISensorCommand *getStartDailyKkhCommand() {
        #error "Not implemented in SONOFF device";
    }

    ISensorCommand *getResetDailyKkhCommand() {
        #error "Not implemented in SONOFF device";
    }

    IScalarSensor<SensorValue<float>> *getLineFrequencySensor() {
        #error "Not implemented in SONOFF device";
    }

    IScalarSensor<SensorValue<float>> *getPowerFactorSensor() {
        #error "Not implemented in SONOFF device";
    }

#elif CS_PLATFORM == CS_P_ESP32
    #include "../interfaces/IUART.h"
    #include "../interfaces/ISensorCommand.h"
    #include "../hardware/io/UARTInterface.hpp"
    #include "../hardware/devices/MCP39F511/MCP39F511Consts.h"

    #include "../hardware/sensors/S6MCP39F511StartDailyKwh.hpp"
    #include "../hardware/sensors/S6MCP39F511ResetDailyKwh.hpp"
    #include "../hardware/sensors/S6MCP39F511U32Register.hpp"
    #include "../hardware/sensors/S6MCP39F511U16Register.hpp"
    #include "../hardware/sensors/S6MCP39F511U64Register.hpp"

    IUART *_uart = nullptr;

    IUART *setupUART() {
        if(_uart == nullptr) {
            UARTConfig config;
            config.baud_rate = 115200;
            config.num_data_bits = 8;
            config.parity = UART_PARITY_NONE;
            config.stop_bits = UART_STOP_BITS_1;

            //tx = 36, rx = 4
            _uart = dynamic_cast<IUART*>(new UARTInterface(2, config, 36, 4));
        }
        return _uart;
    }

    IScalarSensor<SensorValue<float>> *getActivePowerSensor() {
        IUART *uart = setupUART();
        return new S6MCP39F511U32Register(uart, REG_ACTIVE_POWER);
    }

    IScalarSensor<SensorValue<float>> *getReactivePowerSensor() {
        IUART *uart = setupUART();
        return new S6MCP39F511U32Register(uart, REG_REACTIVE_POWER);
    }

    IScalarSensor<SensorValue<float>> *getVoltageSensor() {
        IUART *uart = setupUART();
        return new S6MCP39F511U16Register(uart, REG_VOLTAGE);
    }

    IScalarSensor<SensorValue<float>> *getDailyKwhSensor() {
        IUART *uart = setupUART();
        return new S6MCP39F511U64Register(uart, REG_IMP_ACTIVE_CNT);
    }

    IScalarSensor<SensorValue<float>> *getCurrentSensor() {
        IUART *uart = setupUART();
        return new S6MCP39F511U32Register(uart, REG_CURRENT);
    }

    IScalarSensor<SensorValue<float>> *getLineFrequencySensor() {
        IUART *uart = setupUART();
        return new S6MCP39F511U16Register(uart, REG_FREQUENCY);
    }

    IScalarSensor<SensorValue<float>> *getPowerFactorSensor() {
        IUART *uart = setupUART();
        return new S6MCP39F511U16Register(uart, REG_POWER_FACTOR);
    }

    IScalarSensor<SensorValue<float>> *getReadU32Register(McpRegister<float, uint32_t> regData) {
        IUART *uart = setupUART();
        return new S6MCP39F511U32Register(uart, regData);
    }

    IScalarSensor<SensorValue<float>> *getReadU16Register(McpRegister<float, uint16_t> regData) {
        IUART *uart = setupUART();
        return new S6MCP39F511U16Register(uart, regData);
    }

    ISensorCommand *getStartDailyKkhCommand() {
        IUART *uart = setupUART();
        return new S6MCP39F51StartDailyKwh(uart);
    }

    ISensorCommand *getResetDailyKkhCommand() {
        IUART *uart = setupUART();
        return new S6MCP39F511ResetDailyKwh(uart);
    }

#endif



