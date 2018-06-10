//
// Created by Fabrizio Guglielmino on 16/09/17.
//

#pragma once

#define FIRMWARE_APP_VERSION "1.0.4"

#define MQTT_MESSAGE_SIZE       60


#ifdef DEV_FRESNEL
    #include "consts.fresnel.h"
#endif

#ifdef DEV_POWR2
    #include "consts.powr2.h"
#endif