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
            return get_cfg()->s6fresnel.update_interval;
        }

        const char *location() {
            return get_cfg()->s6fresnel.location;
        }

        const char *name() {
            return get_cfg()->s6fresnel.name;
        }
    };

    S6Fresnel s6Fresnel;

public:
    Settings() {

    }

    const char *deviceId() {
        return get_cfg()->device.id;
    }

    S6Fresnel s6fresnel() {
        return s6Fresnel;
    }


};