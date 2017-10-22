//
// Created by Fabrizio Guglielmino on 16/09/17.
//

#pragma once

#define USE_OLD_CFG

#include "mgos.h"

class Settings {

private:

    class S6Fresnel {

    public:
        int updateInterval() {
#ifdef USE_OLD_CFG
            return get_cfg()->s6fresnel.update_interval;
#else
            return mgos_sys_config_get_s6fresnel_update_interval();
#endif
        }

        const char *location() {
#ifdef USE_OLD_CFG
            return get_cfg()->s6fresnel.location;
#else
            return mgos_sys_config_get_s6fresnel_location();
#endif
        }

        const char *name() {
#ifdef USE_OLD_CFG
            return get_cfg()->s6fresnel.name;
#else
            return mgos_sys_config_get_s6fresnel_name();
#endif
        }
    };

    S6Fresnel s6Fresnel;

public:
    Settings() {

    }

    const char *deviceId() {
#ifdef USE_OLD_CFG
        return get_cfg()->device.id;;
#else
        return mgos_sys_config_get_device_id();
#endif
    }

    S6Fresnel s6fresnel() {
        return s6Fresnel;
    }


};