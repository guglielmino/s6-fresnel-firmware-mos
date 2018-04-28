#ifndef __STARTKWHACCUMULATOR_H
#define __STARTKWHACCUMULATOR_H

#include <vector>
#include "../MCP39F511Utils.hpp"
#include "ICommandFrame.hpp"

/**
 * Start the MCP39F511 Kwh accumulator
 */
class StartKWhAccumulator : public ICommandFrame {
public:
    std::vector<uint8_t> frame() {
        return prepareWriteFrame(MCP_REG_RST_ENERGY_CNT, { 0x00, 0x80 });
    }

    WaitForType waitFor() {
        return WaitForType::ACK_ONLY;
    }
};

#endif