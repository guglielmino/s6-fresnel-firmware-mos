//
// Created by Fabrizio Guglielmino on 15/10/17.
//

#pragma once

struct SPISettings {
    SPISettings();
    SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode);
    uint32_t clock;
    uint8_t bit_order;
    uint8_t mode;
};


class ISPI {
public:
    virtual ~ISPI() = default;

    virtual void begin()  = 0;
    virtual void end() = 0;

    virtual void setBitOrder(uint8_t bitOrder) = 0;
    virtual void setDataMode(uint8_t dataMode) = 0;
    virtual void setFrequency(uint32_t freq) = 0;

    virtual void beginTransaction(SPISettings config) = 0;
    virtual void endTransaction() = 0;

    virtual uint8_t transfer(uint8_t data) = 0;
    virtual uint16_t transfer16(uint16_t data) = 0;
    virtual uint32_t transfer32(uint32_t data) = 0;

    virtual void transfer(void *data, size_t count) = 0;
    virtual void transferBytes(const uint8_t *data, uint8_t *out, uint32_t size) = 0;
    virtual void write(uint8_t data) = 0;
    virtual void write16(uint16_t data) = 0;
    virtual void write32(uint32_t data) = 0;
    virtual void writeBytes(const uint8_t *data, uint32_t size) = 0;
};
