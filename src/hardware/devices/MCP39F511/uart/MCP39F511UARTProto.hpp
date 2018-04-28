#ifndef __MCP39F511PUARTPROTO_H
#define __MCP39F511PUARTPROTO_H

#include <vector>
#include <functional>
#include "IMCP39F511UARTProto.h"
#include "../commands/ICommandFrame.hpp"
#include "../../../io/IUART.h"


/**
 * MCP39F511 UART protocol. Manage UART communication with MCP39F511 chip
 */
class MCP39F511UARTProto : public IMCP39F511UARTProto {

private:
    readCallback_t _finish_cb;

    enum FillState {
        Idle = 0,
        Reading
    };


    static const uint8_t ACK = 0x06;
    static const uint8_t NACK = 0x15;
    static const uint8_t CSFAIL = 0x51;

    void fillInternalAccumulator(const uint8_t *buffer, size_t size) {
        for (int i = 0; i < size; ++i) {
            accumulator.push_back(buffer[i]);
            bytesLeft--;
        }
    }



    FillState fillState = FillState::Idle;
    std::vector <uint8_t> accumulator;
    uint8_t bytesLeft; // bytes left to the end of the receiving buffer
    ICommandFrame::WaitForType waitFor;
    IUART *_uart;


    void process(uint8_t *buffer, size_t size) {
        LOG(LL_DEBUG, ("McpUARTProtoReader::process 0x%x", (uint8_t) buffer[0]));
        switch (fillState) {
            case FillState::Idle: {
                // Command interpreter
                if (buffer[0] == ACK) {
                    if(waitFor == ICommandFrame::WaitForType::ACK_DATA_CHECKSUM) {
                        bytesLeft = buffer[1];
                        LOG(LL_DEBUG, ("ACK (%d - %d)", bytesLeft, size));
                        // Sometime size > byteLeft (it's due to serial communications, more data than the declared size
                        // at protocol level). In that case the byteLeft is the total number of bytes to get
                        fillInternalAccumulator(buffer, MIN(size,  bytesLeft));
                        LOG(LL_DEBUG, ("ACK LEFT (%d)", bytesLeft));
                    }
                    else {
                        bytesLeft = 0;
                    }
                    fillState = Reading;
                } else if (buffer[0] == NACK) {
                    LOG(LL_DEBUG, ("NACK\n"));
                    _finish_cb(RespType::Nack, nullptr, 0);
                } else if (buffer[0] == CSFAIL) {
                    LOG(LL_DEBUG, ("CSFAIL\n"));
                    accumulator.clear();
                    fillState = Idle;
                    bytesLeft = 0;
                    _finish_cb(RespType::ChecksumErr, nullptr, 0);
                } else {
                    LOG(LL_DEBUG, ("*** Packet - UNKNOW 0x%x", (uint8_t) buffer[0]));
                }
            }
                break;
            case FillState::Reading: {
                fillInternalAccumulator(buffer, size);
            }
                break;
        }

        if (bytesLeft == 0 && fillState == Reading) {
            _uart->flush();
            fillState = Idle;
            _finish_cb(RespType::Ack, reinterpret_cast<uint8_t *>(accumulator.data()), accumulator.size());
            accumulator.clear();
        }


    }

    void writeFrame(std::vector <uint8_t> frame) {
        int frameSize = frame.size();

        for (unsigned int i = 0; i < frameSize; ++i) {
            _uart->write(&frame[i], 1);
            _uart->flush();
            mgos_usleep(1000);
        }
    }


public:

    MCP39F511UARTProto(IUART *uart) : _uart(uart) {
        bytesLeft = 0;
        fillState = FillState::Idle;
    }


    bool sendCommand(ICommandFrame& command) {
        if(fillState != Idle) {
            LOG(LL_DEBUG, ("UART operation in progress"));
            return false;
        }
        std::vector <uint8_t> frame = command.frame();
        // Set the waitFot type for the processing command
        waitFor = command.waitFor();
        writeFrame(frame);
        return true;
    }



    void readAsync(readCallback_t cb) {
        _finish_cb = cb;
        _uart->readAsync([&, this]() {
            size_t rx_av = _uart->readAvail();
            if (rx_av > 0) {
                uint8_t locbuffer[rx_av];
                memset(locbuffer, 0, rx_av);

                _uart->read(locbuffer, rx_av);
                this->process(locbuffer, rx_av);
            }
        });
    }
};

#endif