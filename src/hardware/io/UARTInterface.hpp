//
// Created by Fabrizio Guglielmino on 21/10/17.
//

#pragma once

#include "common/platform.h"

#include "mgos.h"
#include "mgos_uart.h"

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
    uint8_t baud_rate;
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

class UARTInterface {
public:
    typedef void (*read_async_callback_t)(char *buffer, size_t len);

private:
    uint8_t _uartNum;
    read_async_callback_t _read_async_callback;

    static void _internal_read_async_callback(int uart_no, void *arg) {
        UARTInterface *me = (UARTInterface *)arg;

        size_t rx_av = mgos_uart_read_avail(uart_no);
        if (rx_av > 0) {
            struct mbuf rxb;
            mbuf_init(&rxb, 0);
            mgos_uart_read_mbuf(uart_no, &rxb, rx_av);
            if (rxb.len > 0) {
                LOG(LL_DEBUG, ("DBG: DISPATCH READ %.*s", (int) rxb.len, rxb.buf));
                me->_read_async_callback(rxb.buf, rxb.len);
            }
            mbuf_free(&rxb);
        }
    }

public:

    UARTInterface(int uartNum, UARTConfig config, uint8_t txgpio, uint8_t rxgpio) : _uartNum(uartNum) {
        struct mgos_uart_config cfg;
        mgos_uart_config_set_defaults(_uartNum, &cfg);

        cfg.baud_rate = config.baud_rate;
        cfg.num_data_bits = config.num_data_bits;
        cfg.parity = toMosParity(config.parity);
        cfg.stop_bits = toMosStopBits(config.stop_bits);
        cfg.dev.tx_gpio = rxgpio; // 4
        cfg.dev.rx_gpio = txgpio; // 36

        mgos_uart_configure(_uartNum, &cfg);

        mgos_uart_set_rx_enabled(_uartNum, true);
    }


    size_t write(const void* buffer, size_t len) {
        return mgos_uart_write(_uartNum, buffer, len);
    }

    size_t read(void *buf, size_t len) {
        return mgos_uart_read(_uartNum, buf, len);
    }

    void readAsync(read_async_callback_t cb) {
        _read_async_callback = cb;
        mgos_uart_set_dispatcher(_uartNum, _internal_read_async_callback, this);
    }

    size_t readAvail() {
        return mgos_uart_read_avail(_uartNum);
    }

    size_t writeAvail() {
        return mgos_uart_write_avail(_uartNum);
    }

};