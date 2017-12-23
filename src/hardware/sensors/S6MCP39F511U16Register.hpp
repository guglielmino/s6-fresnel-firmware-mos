//
// Created by Fabrizio Guglielmino on 13/12/17.
//


#pragma once

#include "hardware/devices/MCP39F511/MCP39F511Consts.h"
#include "hardware/devices/MCP39F511/MCP39F511Utils.hpp"

#include "interfaces/IADCReader.h"
#include "interfaces/IScalarSensor.h"
#include "SensorValue.hpp"

class S6MCP39F511U16Register : public IScalarSensor<SensorValue<float>>, MCP39F511Utils {
private:
    IUART *_uart;
    McpRegister<float, uint16_t> _regData;

public:
    S6MCP39F511U16Register(IUART *uart, McpRegister<float, uint16_t> regData) : _uart(uart),
                                                                               _regData(regData) {

    }

    SensorValue<float> readValue() {
        SensorValue<float> ret(0, false);
        char buffer[_regData.bufferSize];

        bool success = readRegister(_uart, _regData.reg, 2, buffer, _regData.bufferSize);

        if (success) {
            uint16_t Value = dataTypes.u16(buffer, 0);
            ret.setValue(_regData.rawToValue(Value));
        } else {
            LOG(LL_DEBUG, ("*** READ FAILED!!!"));
        }

        return ret;
    }
};