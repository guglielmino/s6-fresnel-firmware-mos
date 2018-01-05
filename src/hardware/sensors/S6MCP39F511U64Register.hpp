#pragma once

#include <functional> 

#include "hardware/devices/MCP39F511/MCP39F511Utils.hpp"

#include "interfaces/IADCReader.h"
#include "interfaces/IScalarSensor.h"
#include "SensorValue.hpp"

class S6MCP39F511U64Register : public IScalarSensor<SensorValue<float>>, MCP39F511Utils {
private:
    IUART *_uart;
    McpRegister<float, uint64_t> _regData;
  
public:
    S6MCP39F511U64Register(IUART *uart, McpRegister<float, uint64_t> regData) : _uart(uart),
                                                                               _regData(regData) {

    }

    SensorValue<float> readValue() {
        SensorValue<float> ret(0, false);
        char buffer[_regData.bufferSize];

        bool success = readRegister(_uart,  _regData.reg, 8, buffer,  _regData.bufferSize);

        if (success) {
            uint64_t Value = dataTypes.u64(buffer, 0);
            ret.setValue(_regData.rawToValue(Value));
        } else {
            LOG(LL_DEBUG, ("*** READ REGISTER FAILED!!!"));
        }

        return ret;
    }
};