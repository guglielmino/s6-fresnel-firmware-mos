//
// Created by Fabrizio Guglielmino on 16/09/17.
//

#pragma once

#include <string>

//#define USE_OLD_CFG

#include "mgos.h"

class Settings {

private:

    class S6Fresnel {

    public:
       
        const char *group() {
#ifdef USE_OLD_CFG
            return get_cfg()->s6fresnel.group;
#else
            return mgos_sys_config_get_s6fresnel_group();
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

    class MQTTSettings {
    public:
        void server(std::string serverAddress) {
            mgos_sys_config_set_mqtt_server(serverAddress.c_str());
        }

        void lwtTopic(std::string lwtTopic) {
            mgos_sys_config_set_mqtt_will_topic(lwtTopic.c_str());
        }

        void lwtMessage(std::string lwtMessage) {
            mgos_sys_config_set_mqtt_will_message(lwtMessage.c_str());
        }

    };

    MQTTSettings mqttSettings;

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

    // Sub config objects
    S6Fresnel s6fresnel() {
        return s6Fresnel;
    }

    MQTTSettings mqtt() {
        return mqttSettings;
    }


};