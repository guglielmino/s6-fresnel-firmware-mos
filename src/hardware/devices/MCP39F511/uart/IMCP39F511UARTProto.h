#pragma once

#include <vector>
#include <functional>
#include "../commands/ICommandFrame.hpp"

class IMCP39F511UARTProto {
public:
    enum RespType {
        Ack,
        Nack,
        ChecksumErr
    };

    virtual void sendCommand(ICommandFrame& command) = 0;
    virtual void readAsync(std::function<void(RespType respType, const uint8_t *buffer, size_t size)> cb) = 0;
};