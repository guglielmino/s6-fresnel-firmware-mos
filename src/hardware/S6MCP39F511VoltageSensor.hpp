//
// Created by Fabrizio Guglielmino on 29/10/17.
//

#pragma once

#include "../hardware/devices/MCP39F511/MCP39F511Utils.hpp"
#include "../hardware/devices/MCP39F511/MCP39F511Command.hpp"
#include "../hardware/devices/MCP39F511/AddressPointerCmd.hpp"
#include "../hardware/devices/MCP39F511/RegisterReadCmd.hpp"

#include "../interfaces/IScalarSensor.h"
#include "../interfaces/IUART.h"

#define MAX_MCP39F511_BUFF_SIZE 35
#define VOLTAGE_RESP_BUFFER_SIZE 5

class S6MCP39F511VoltageSensor : public IScalarSensor<float>, MCP39F511Utils {
private:
    IUART *_uart;

public:
    S6MCP39F511VoltageSensor(IUART *uart)  : _uart(uart) {

    }

    float readValue() {
        float ret = 0.0;
        AddressPointerCmd setPointer(0x00, 0x06);
        RegisterReadCmd readRegister(0x02);

        std::vector<MCP39F511Command *> commands;
        commands.push_back(&setPointer);
        commands.push_back(&readRegister);
        std::vector<uint8_t> frame = makeFrame(commands);
        int frameSize = frame.size();

        for (unsigned int i = 0; i < frameSize; ++i) {
            _uart->write(&frame[i], 1);
            _uart->flush();
            mgos_usleep(1000);
        }
        mgos_usleep(5000);

        char buff[VOLTAGE_RESP_BUFFER_SIZE];
        size_t read = _uart->read(buff, VOLTAGE_RESP_BUFFER_SIZE);
        if(read > 0 && checkResp(buff)) {
            uint16_t Vrms = readInt(buff, 0);
            ret = (Vrms / 10.0);
        }

       return ret;
    }
};