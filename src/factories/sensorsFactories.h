#pragma once

#include "../interfaces/IScalarSensor.h"
#include "../interfaces/IADCReader.h"

#if CS_PLATFORM == CS_P_ESP8266
    #include "../hardware/sensors/SONOFFPowerSensor.h"
    #include "../hardware/sensors/SONOFFDailyKwh.h"

    IScalarSensor<float> *getPowerSensor() {
        return new SONOFFPowerSensor();
    }

    IScalarSensor<float> *getVoltageSensor() {
        #error "No Voltage Sensor Implemented for SONOFF device";
    }

    IScalarSensor<float> *getDailyKwhSensor() {
        return new SONOFFDailyKwh();
    }

    ISensorCommand *getStartDailyKkhCommand() {
        #error "Not implemented in SONOFF device";
    }

    ISensorCommand *getResetDailyKkhCommand() {
        #error "Not implemented in SONOFF device";
    }

#elif CS_PLATFORM == CS_P_ESP32
    #include "../interfaces/IUART.h"
    #include "../interfaces/ISensorCommand.hpp"
    #include "../hardware/io/UARTInterface.hpp"
    #include "../hardware/sensors/S6MCP39F511PowerSensor.hpp"
    #include "../hardware/sensors/S6MCP39F511VoltageSensor.hpp"
    #include "../hardware/sensors/S6MCP39F511DailyKwh.hpp"
    #include "../hardware/sensors/S6MCP39F511StartDailyKwh.hpp"
    #include "../hardware/sensors/S6MCP39F511ResetDailyKwh.hpp"

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

    IScalarSensor<float> *getPowerSensor() {
        IUART *uart = setupUART();
        return new S6MCP39F511PowerSensor(uart);
    }

    IScalarSensor<float> *getVoltageSensor() {
        IUART *uart = setupUART();
        return new S6MCP39F511VoltageSensor(uart);
    }

    IScalarSensor<float> *getDailyKwhSensor() {
        IUART *uart = setupUART();
        return new S6MCP39F511DailyKwh(uart);
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



