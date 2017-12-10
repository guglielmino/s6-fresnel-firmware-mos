//
// Created by Fabrizio Guglielmino on 29/10/17.
//

#pragma once

#include "hardware/devices/MCP39F511/MCP39F511Utils.hpp"
#include "hardware/devices/MCP39F511/MCP39F511Command.hpp"
#include "hardware/devices/MCP39F511/AddressPointerCmd.hpp"
#include "hardware/devices/MCP39F511/RegisterReadCmd.hpp"

#include "interfaces/IScalarSensor.h"
#include "interfaces/IUART.h"
#include "SensorValue.hpp"

class S6MCP39F511VoltageSensor : public IScalarSensor<SensorValue<float>>, MCP39F511Utils {
private:
    IUART *_uart;
    static const int BUFFER_SIZE = 5;

public:
    S6MCP39F511VoltageSensor(IUART *uart)  : _uart(uart) {

    }

    SensorValue<float> readValue() {
        SensorValue<float> ret(0.0, false);
        char buffer[S6MCP39F511VoltageSensor::BUFFER_SIZE];
        bool success = readRegister(_uart, MCP_REG_VOLTS, 2, buffer, S6MCP39F511VoltageSensor::BUFFER_SIZE);

        if(success) {
            uint16_t Vrms = dataTypes.u16(buffer, 0);
            ret.setValue((Vrms / 10.0));
        } else {
            LOG(LL_DEBUG, ("*** READ VOLTS FAILED!!!"));
        }

       return ret;
    }
};