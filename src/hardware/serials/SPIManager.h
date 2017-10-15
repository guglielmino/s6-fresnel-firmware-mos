//
// Created by Fabrizio Guglielmino on 15/10/17.
// Inspired by Arduino Mongoose SPI Interface https://github.com/mongoose-os-libs/arduino-spi/blob/master/include/SPI.h
//

#pragma once

#include "../../interfaces/ISPI.h"

#ifndef LSBFIRST
#define LSBFIRST 0
#endif
#ifndef MSBFIRST
#define MSBFIRST 1
#endif

#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

#define SPI_HAS_TRANSACTION 1
#define SPI_DEFAULT_FREQ 1000000

class SPIManager : ISPI {
private:
    struct mgos_spi *spi_;
    struct mgos_spi_txn txn_;

public:
    void begin() {
        if (spi_ == nullptr) spi_ = mgos_spi_get_global();
    }

    void end() {
    }

    void setBitOrder(uint8_t bitOrder) {
        (void) bitOrder;
    }

    void setDataMode(uint8_t dataMode) {
        txn_.mode = dataMode;
    }

    void setFrequency(uint32_t freq) {
        txn_.freq = freq;
    }

    void beginTransaction(SPISettings settings) {
        setFrequency(settings.clock);
        setBitOrder(settings.bit_order);
        setDataMode(settings.mode);
    }

    void endTransaction() {
    }

    uint8_t transfer(uint8_t data) {
        struct mgos_spi_txn txn = txn_;
        if (spi_ == nullptr) return 0;
        txn.fd.tx_data = txn.fd.rx_data = &data;
        txn.fd.len = sizeof(data);
        if (!mgos_spi_run_txn(spi_, true /* full_duplex */, &txn)) data = 0;
        return data;
    }

    uint16_t transfer16(uint16_t data) {
        struct mgos_spi_txn txn = txn_;
        if (spi_ == nullptr) return 0;
        data = htons(data);
        txn.fd.tx_data = txn.fd.rx_data = &data;
        txn.fd.len = sizeof(data);
        if (!mgos_spi_run_txn(spi_, true /* full_duplex */, &txn)) data = 0;
        data = ntohs(data);
        return data;
    }

    uint32_t transfer32(uint32_t data) {
        struct mgos_spi_txn txn = txn_;
        if (spi_ == nullptr) return 0;
        data = htonl(data);
        txn.fd.tx_data = txn.fd.rx_data = &data;
        txn.fd.len = sizeof(data);
        if (!mgos_spi_run_txn(spi_, true /* full_duplex */, &txn)) data = 0;
        data = ntohl(data);
        return data;
    }

    void transfer(void *data, size_t count) {
        struct mgos_spi_txn txn = txn_;
        if (spi_ == nullptr) return;
        txn.fd.tx_data = txn.fd.rx_data = data;
        txn.fd.len = count;
        mgos_spi_run_txn(spi_, true /* full_duplex */, &txn);
    }

    void transferBytes(const uint8_t *data, uint8_t *out, uint32_t size) {
        struct mgos_spi_txn txn = txn_;
        if (spi_ == nullptr) return;
        txn.fd.tx_data = data;
        txn.fd.rx_data = out;
        txn.fd.len = size;
        mgos_spi_run_txn(spi_, true /* full_duplex */, &txn);
    }

    void write(uint8_t data) {
        struct mgos_spi_txn txn = txn_;
        if (spi_ == nullptr) return;
        txn.hd.tx_data = &data;
        txn.hd.tx_len = sizeof(data);
        mgos_spi_run_txn(spi_, false /* full_duplex */, &txn);
    }

    void write16(uint16_t data) {
        struct mgos_spi_txn txn = txn_;
        if (spi_ == nullptr) return;
        data = htons(data);
        txn.hd.tx_data = &data;
        txn.hd.tx_len = sizeof(data);
        mgos_spi_run_txn(spi_, false /* full_duplex */, &txn);
    }

    void write32(uint32_t data) {
        struct mgos_spi_txn txn = txn_;
        if (spi_ == nullptr) return;
        data = htonl(data);
        txn.hd.tx_data = &data;
        txn.hd.tx_len = sizeof(data);
        mgos_spi_run_txn(spi_, false /* full_duplex */, &txn);
    }

    void writeBytes(const uint8_t *data, uint32_t size) {
        struct mgos_spi_txn txn = txn_;
        if (spi_ == nullptr) return;
        txn.hd.tx_data = data;
        txn.hd.tx_len = size;
        mgos_spi_run_txn(spi_, false /* full_duplex */, &txn);
    }
};