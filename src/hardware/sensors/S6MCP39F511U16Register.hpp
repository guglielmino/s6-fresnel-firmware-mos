//
// Created by Fabrizio Guglielmino on 13/12/17.
//


#pragma once

#include "hardware/devices/MCP39F511/MCP39F511Utils.hpp"

#include "interfaces/IADCReader.h"
#include "interfaces/IScalarSensor.h"
#include "SensorValue.hpp"

class S6MCP39F511U16Register : public IScalarSensor<SensorValue<float>>, MCP39F511Utils {
private:
    IUART *_uart;
    uint16_t _mcpRegister;
    float _divider;
    static const int BUFFER_SIZE = 5;

public:
    S6MCP39F511U16Register(IUART *uart, uint16_t mcrRegister, float divider) : _uart(uart),
                                                                               _mcpRegister(mcrRegister),
                                                                               _divider(divider) {

    }


    SensorValue<float> readValue() {
        SensorValue<float> ret(0, false);
        char buffer[S6MCP39F511U16Register::BUFFER_SIZE];

        bool success = readRegister(_uart, _mcpRegister, 2, buffer, S6MCP39F511U16Register::BUFFER_SIZE);

        if (success) {
            uint16_t Freq = dataTypes.u16(buffer, 0);
            ret.setValue((Freq / _divider));
        } else {
            LOG(LL_DEBUG, ("*** READ FAILED!!!"));
        }

        return ret;
    }
};