//
// Created by Fabrizio Guglielmino on 16/09/17.
//

#pragma once


#include <map>
#include "mgos.h"
#include "mgos_mqtt.h"

namespace S6MqttModule {

    class MQTTManager {
    public:
        enum MqttEvent {
            NoOp, Connected, ConnectError, Disconnected
        };

    private:
        typedef void (*event_callback_t)(void);
        bool isConnected = false;

        static void mqtt_ev_handler(struct mg_connection *c, int ev, void *p, void *user_data) {
            MQTTManager *mqttManager = (MQTTManager *) user_data;
            struct mg_mqtt_message *msg = (struct mg_mqtt_message *) p;
            if (ev == MG_EV_MQTT_CONNACK) {
                LOG(LL_INFO, ("CONNACK: %d", msg->connack_ret_code));
                mqttManager->isConnected = true;

                if(mqttManager->callbacks[MqttEvent::Connected]) {
                    mqttManager->callbacks[MqttEvent::Connected]();
                }
                //if (get_cfg()->mqtt.pub == NULL) {
                //LOG(LL_ERROR, ("Run 'mos config-set mqtt.pub=... '"));
                //} else {
                //pub(c, "{timestamp:%.3lf, mem_free:%d}", mg_time(), mgos_get_free_heap_size() );  /* post uptime */
                //}
            } else if (ev == MG_EV_CLOSE) {
                mqttManager->isConnected = false;
                if(mqttManager->callbacks[MqttEvent::ConnectError]) {
                    mqttManager->callbacks[MqttEvent::ConnectError]();
                }
            }

            (void) c;
        }

        struct mg_connection *_mqttConn = NULL;
        std::map<MQTTManager::MqttEvent, event_callback_t> callbacks;

    public:

        MQTTManager() {
            mgos_mqtt_add_global_handler(MQTTManager::mqtt_ev_handler, this);
        }


        void setEventCallback(MqttEvent event, event_callback_t cb) {
            callbacks[event] = cb;
        }

        void publish(const char *topic, const char *payload, int len, int qos = 0, bool retain = false) {
            if (isConnected) {
                mgos_mqtt_pub(topic, (const void *)payload, len, qos, retain);
            } else {
                LOG(LL_DEBUG, ("Trying yo publish on %s with MQTT not connected", topic));
            }
        }

    };

}