#pragma once

#include <vector>
#include <functional>
#include "../commands/ICommandFrame.hpp"

enum RespType {
    Ack,
    Nack,
    ChecksumErr
};
typedef std::function<void(RespType respType, const uint8_t *buffer, size_t size)> readCallback_t;

class IMCP39F511UARTProto {
public:


    virtual bool sendCommand(ICommandFrame& command) = 0;
    virtual void readAsync(readCallback_t cb) = 0;
};