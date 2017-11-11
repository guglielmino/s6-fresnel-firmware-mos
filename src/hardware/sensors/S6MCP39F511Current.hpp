//
// Created by Fabrizio Guglielmino on 11/11/17.
//

#pragma once

#include "hardware/devices/MCP39F511/MCP39F511Utils.hpp"

#include "interfaces/IADCReader.h"
#include "interfaces/IScalarSensor.h"

#define CURRENT_RESP_BUFFER_SIZE 7

class S6MCP39F511Current : public IScalarSensor<float>, MCP39F511Utils {
private:
    IUART *_uart;

public:
    S6MCP39F511Current(IUART *uart) : _uart(uart) {

    }

    /**
     * Aggiungere gestione con readAvail e eventualmente millis di attesa fincheè minore di 0 per ogni commando
     * @return
     */
    float readValue() {
        float ret = 0;
        char buffer[CURRENT_RESP_BUFFER_SIZE];

        bool success = readRegister(_uart, MCP_REG_CURRENT_RMS, 4, buffer, CURRENT_RESP_BUFFER_SIZE);

        if (success) {
            uint32_t Current = u32(buffer, 0);
            ret = (Current / 10000.0);
        } else {
            LOG(LL_DEBUG, ("*** READ CURRENT FAILED!!!"));
        }

        return ret;
    }
};