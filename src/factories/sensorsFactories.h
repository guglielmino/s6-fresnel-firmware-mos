#pragma once

#include "../interfaces/IScalarSensor.h"
#include "../interfaces/IADCReader.h"

#if CS_PLATFORM == CS_P_ESP8266
    #include "../hardware/SONOFFPowerSensor.h"
    #include "../hardware/SONOFFDailyKwh.h"

    IScalarSensor<float> *getPowerSensor() {
        return new SONOFFPowerSensor();
    }

    IScalarSensor<float> *getVoltageSensor() {
        #error "No Voltage Sensor Implemented for SONOFF device";
    }

    IScalarSensor<uint64_t> *getDailyKwhSensor() {
        return new SONOFFDailyKwh();
    }
#elif CS_PLATFORM == CS_P_ESP32
    #include "../interfaces/IUART.h"
    #include "../hardware/io/UARTInterface.hpp"
    #include "../hardware/S6MCP39F511PowerSensor.hpp"
    #include "../hardware/S6MCP39F511VoltageSensor.hpp"
    #include "../hardware/S6MCP39F511DailyKwh.hpp"

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

    IScalarSensor<unsigned long> *getDailyKwhSensor() {
        IUART *uart = setupUART();
        return new S6MCP39F511DailyKwh(uart);
    }

#endif



