#pragma once

#include "hardware/devices/MCP39F511/MCP39F511Utils.hpp"

#include "interfaces/IADCReader.h"
#include "interfaces/IScalarSensor.h"
#include "SensorValue.hpp"

class S6MCP39F511U32Register : public IScalarSensor<SensorValue<float>>, MCP39F511Utils {
private:
    IUART *_uart;
    uint16_t _mcpRegister;
    float _divider;
    static const int BUFFER_SIZE = 7;


public:
    S6MCP39F511U32Register(IUART *uart, uint16_t mcrRegister, float divider) : _uart(uart),
                                                                               _mcpRegister(mcrRegister),
                                                                               _divider(divider)
    {

    }

    SensorValue<float> readValue() {
        SensorValue<float> ret(0, false);
        char buffer[S6MCP39F511U32Register::BUFFER_SIZE];

        bool success = readRegister(_uart, _mcpRegister, 4, buffer, S6MCP39F511U32Register::BUFFER_SIZE);

        if (success) {
            uint32_t Current = dataTypes.u32(buffer, 0);
            ret.setValue((Current / _divider));
        } else {
            LOG(LL_DEBUG, ("*** READ REGISTER FAILED!!!"));
        }

        return ret;
    }
};