//
// Created by Fabrizio Guglielmino on 10/10/17.
//

#pragma once

#include "../interfaces/IScalarSensor.h"
#include "../interfaces/IUART.h"

#define MAX_MCP39F511_BUFF_SIZE 35


class S6MCP39F511PowerSensor : public IScalarSensor<float> {
private:
    const IUART &_uart;

public:
    S6MCP39F511PowerSensor(const IUART &uart)  : _uart(uart) {

    }

    float readValue() {

        /*

         NOTE: WRITE BYTE BY BYTE WIth 1ms delay
        for (unsigned int i = 0; i < sizeof(cmd); ++i) {
            uart->write(&cmd[i], 1);
            uart->flush();
            mgos_usleep(1000);
         }
    }
         */

        return 0;
    }
};


