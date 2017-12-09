//
// Created by Fabrizio Guglielmino on 10/10/17.
//

#pragma once

#include "hardware/devices/MCP39F511/MCP39F511Utils.hpp"

#include "SensorValue.hpp"
#include "interfaces/IScalarSensor.h"
#include "interfaces/IUART.h"


class S6MCP39F511PowerSensor : public IScalarSensor<SensorValue<float>>, MCP39F511Utils {
public:
    typedef enum {
        ACTIVE,
        REACTIVE,
        APPARENT
    } PowerType;

private:
    IUART *_uart;
    static const int BUFFER_SIZE = 7;
    std::map<PowerType, uint16_t> _registers;
    PowerType _type;

public:

    S6MCP39F511PowerSensor(IUART *uart, PowerType type)  : _uart(uart), _type(type) {
        _registers[S6MCP39F511PowerSensor::ACTIVE] = MCP_REG_ACTIVE_POWER;
        _registers[S6MCP39F511PowerSensor::REACTIVE] = MCP_REG_REACTIVE_POWER;
        _registers[S6MCP39F511PowerSensor::APPARENT] = MCP_REG_APPARENT_POWER;
    }

    SensorValue<float> readValue() {
        SensorValue<float> ret(0.0, false);
        char buffer[S6MCP39F511PowerSensor::BUFFER_SIZE];
        bool success = readRegister(_uart, _registers[_type], 4, buffer, S6MCP39F511PowerSensor::BUFFER_SIZE);

        if(success) {
            uint32_t power = dataTypes.u32(buffer, 0);
            ret.setValue((power / 100.0));
        }else {
            LOG(LL_DEBUG, ("*** READ POWER FAILED!!!"));
        }

        return ret;
    }
};


