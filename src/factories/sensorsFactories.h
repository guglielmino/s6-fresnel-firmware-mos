#pragma once

#include "../interfaces/IScalarSensor.h"
#include "../interfaces/IADCReader.h"

#if CS_PLATFORM == CS_P_ESP8266
    #include "../hardware/SONOFFPowerSensor.h"
    #include "../hardware/SONOFFDailyKwh.h"
#elif CS_PLATFORM == CS_P_ESP32
    #include "../interfaces/IUART.h"
    #include "../hardware/io/UARTInterface.hpp"
    #include "../hardware/S6MCP39F511PowerSensor.hpp"
    #include "../hardware/S6MCP39F511VoltageSensor.hpp"
    #include "../hardware/S6DailyKwh.h"
#endif

static IUART *_uart = nullptr;

IUART *setupUART() {
    if(_uart == nullptr) {
        UARTConfig config;
        config.baud_rate = 115200;
        config.num_data_bits = 8;
        config.parity = UART_PARITY_NONE;
        config.stop_bits = UART_STOP_BITS_1;

        //tx = 36, rx = 4
        _uart = (IUART*) new UARTInterface(2, config, 36, 4);
    }
    return _uart;
}

IScalarSensor<float> *getPowerSensor() {
#if CS_PLATFORM == CS_P_ESP8266
    return new SONOFFPowerSensor();
#elif CS_PLATFORM == CS_P_ESP32
    IUART *uart = setupUART();
    return new S6MCP39F511PowerSensor(uart);
#endif
}

IScalarSensor<float> *getVoltageSensor() {
#if CS_PLATFORM == CS_P_ESP8266
    #error "No Voltage Sensor Implemented for SONOFF device";
#elif CS_PLATFORM == CS_P_ESP32
    IUART *uart = setupUART();
    return new S6MCP39F511VoltageSensor(uart);
#endif
}

IScalarSensor<unsigned long> *getDailyKwhSensor() {
#if CS_PLATFORM == CS_P_ESP8266
    return new SONOFFDailyKwh();
#elif CS_PLATFORM == CS_P_ESP32
    return new S6DailyKwh();
#endif
}