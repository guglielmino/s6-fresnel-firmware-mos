//
// Created by Fabrizio Guglielmino on 16/09/17.
//

#pragma once

#include <string>
#include <vector>
#include <stdio.h>

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

        std::vector<const char *> features() {
            char *featuresString;
#ifdef USE_OLD_CFG
            featuresString = get_cfg()->s6fresnel.features;
#else
            featuresString = (char *)mgos_sys_config_get_s6fresnel_features();
#endif
            char * pch;
            std::vector<const char *> feats;
            pch = strtok (featuresString, ",");
            while (pch != NULL)
            {
                feats.push_back(pch);
                pch = strtok (NULL, ",");
            }
            return feats;
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