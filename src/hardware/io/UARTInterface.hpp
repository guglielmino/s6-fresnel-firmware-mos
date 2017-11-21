//
// Created by Fabrizio Guglielmino on 25/10/17.
//

#pragma once

#include "mgos.h"
#include "mgos_uart.h"
#include "../../interfaces/IUART.h"

enum UARTParity {
    UART_PARITY_NONE = 0,
    UART_PARITY_EVEN = 1,
    UART_PARITY_ODD = 2,
};

enum UARTStopBits {
    UART_STOP_BITS_1 = 1, /* So that 1 means 1 bit and 2 means 2. */
    UART_STOP_BITS_2 = 2,
    UART_STOP_BITS_1_5 = 3,
};

struct UARTConfig {
    uint32_t baud_rate;
    uint8_t num_data_bits;
    enum UARTParity parity;
    enum UARTStopBits stop_bits;
};

enum mgos_uart_parity toMosParity(enum UARTParity parity) {
    enum mgos_uart_parity ret = MGOS_UART_PARITY_NONE;
    switch (parity) {
        case UART_PARITY_NONE:
            ret = MGOS_UART_PARITY_NONE;
            break;
        case UART_PARITY_EVEN:
            ret = MGOS_UART_PARITY_EVEN;
            break;
        case UART_PARITY_ODD:
            ret = MGOS_UART_PARITY_ODD;
            break;
    }
    return ret;
}

enum mgos_uart_stop_bits toMosStopBits(enum UARTStopBits stopbits) {
    enum mgos_uart_stop_bits ret = MGOS_UART_STOP_BITS_1;
    switch (stopbits) {
        case UART_STOP_BITS_1:
            ret = MGOS_UART_STOP_BITS_1;
            break;
        case UART_STOP_BITS_2:
            ret = MGOS_UART_STOP_BITS_2;
            break;
        case UART_STOP_BITS_1_5:
            ret = MGOS_UART_STOP_BITS_1_5;
            break;
    }
    return ret;
}

class UARTInterface : public IUART {
private:
    uint8_t _uartNum;
    async_data_available_callback_t _async__data_avail_callback;

    static void _internal_async_data_avail(int uart_no, void *arg) {
        (void)uart_no;
        UARTInterface *me = (UARTInterface *) arg;
        me->_async__data_avail_callback();
    }

public:

    UARTInterface(int uartNum, UARTConfig &config, uint8_t txgpio, uint8_t rxgpio) : _uartNum(uartNum) {
        struct mgos_uart_config cfg;
        mgos_uart_config_set_defaults(_uartNum, &cfg);

        cfg.baud_rate = config.baud_rate;
        cfg.num_data_bits = config.num_data_bits;
        cfg.parity = toMosParity(config.parity);
        cfg.stop_bits = toMosStopBits(config.stop_bits);
#if CS_PLATFORM == CS_P_ESP32
        cfg.dev.tx_gpio = rxgpio; // 4
        cfg.dev.rx_gpio = txgpio; // 36
#else
        (void)txgpio;
        (void)rxgpio;
#endif

        mgos_uart_configure(_uartNum, &cfg);

        mgos_uart_set_rx_enabled(_uartNum, true);
    }


    size_t write(const void *buffer, size_t len) {
        return mgos_uart_write(_uartNum, buffer, len);
    }

    void flush() {
        mgos_uart_flush(_uartNum);
    }

    size_t read(void *buf, size_t len) {
        return mgos_uart_read(_uartNum, buf, len);
    }

    void readAsync(async_data_available_callback_t cb)  {
        _async__data_avail_callback = cb;
        mgos_uart_set_dispatcher(_uartNum, _internal_async_data_avail, this);
    }

    size_t readAvail() {
        return mgos_uart_read_avail(_uartNum);
    }

    size_t writeAvail() {
        return mgos_uart_write_avail(_uartNum);
    }
};
