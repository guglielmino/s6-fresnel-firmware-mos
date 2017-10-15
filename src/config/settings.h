//
// Created by Fabrizio Guglielmino on 16/09/17.
//

#pragma once

#include "mgos.h"

class Settings {

private:

    class S6Fresnel {

    public:
        int updateInterval() {
            return mgos_sys_config_get_s6fresnel_update_interval();
        }

        const char *location() {
            return mgos_sys_config_get_s6fresnel_location();
        }

        const char *name() {
            return mgos_sys_config_get_s6fresnel_name();
        }
    };

    S6Fresnel s6Fresnel;

public:
    Settings() {

    }

    const char *deviceId() {
        return mgos_sys_config_get_device_id();
    }

    S6Fresnel s6fresnel() {
        return s6Fresnel;
    }


};