#pragma once

#include <functional> 

#include "hardware/devices/MCP39F511/MCP39F511Utils.hpp"

#include "interfaces/IADCReader.h"
#include "interfaces/IScalarSensor.h"
#include "SensorValue.hpp"

class S6MCP39F511U32Register : public IScalarSensor<SensorValue<float>>, MCP39F511Utils {
private:
    IUART *_uart;
    McpRegister<float, uint32_t> _regData;
  
public:
    S6MCP39F511U32Register(IUART *uart, McpRegister<float, uint32_t> regData) : _uart(uart),
                                                                               _regData(regData) {

    }

    SensorValue<float> readValue() {
        SensorValue<float> ret(0, false);
        char buffer[_regData.bufferSize];

        bool success = readRegister(_uart, _regData.reg, 4, buffer, _regData.bufferSize);

        if (success) {
            uint32_t Value = dataTypes.u32(buffer, 0);
            ret.setValue(_regData.rawToValue(Value));
        } else {
            LOG(LL_DEBUG, ("*** READ REGISTER FAILED!!!"));
        }

        return ret;
    }
};