#pragma once

#include <vector>
#include "ICommandFrame.hpp"
#include "../MCP39F511Utils.hpp"

class ReadMeteringValues : public ICommandFrame {
public:
    std::vector<uint8_t> frame() {
        return prepareReadFrame(REG_VOLTAGE.reg, 32);
    }

    WaitForType waitFor() {
        return WaitForType::ACK_DATA_CHECKSUM;
    }
};