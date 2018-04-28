//
// Created by Fabrizio Guglielmino on 25/03/18.
//

#ifndef __COMMANDFRAME_H
#define __COMMANDFRAME_H

#include <vector>

class ICommandFrame {
public:
    enum WaitForType {
        ACK_ONLY,
        ACK_DATA_CHECKSUM
    };

    virtual std::vector<uint8_t> frame() = 0;
    virtual WaitForType waitFor() = 0;

};

#endif