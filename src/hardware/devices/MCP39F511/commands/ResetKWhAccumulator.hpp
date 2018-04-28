#ifndef __RESETKWH_H
#define __RESETKWH_H

#include <vector>
#include "../MCP39F511Utils.hpp"
#include "ICommandFrame.hpp"

/**
 * Reset and stop the MCP39F511 Kwh accumulator
 */
class ResetKWhAccumulator : public ICommandFrame {
public:
    std::vector<uint8_t> frame() {
        return prepareWriteFrame(MCP_REG_RST_ENERGY_CNT, { 0x00, 0x00 });
    }

    WaitForType waitFor() {
        return WaitForType::ACK_ONLY;
    }
};

#endif