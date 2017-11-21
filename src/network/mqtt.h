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
            NoOp, Connected, ConnectError, Disconnected, Subscribe
        };

        typedef void (*message_callback_t)(const char *topic, size_t topic_len, const char *msg, size_t msg_len);

    private:
        typedef void (*event_callback_t)(void);

        bool isConnected = false;


        static void mqtt_ev_handler(struct mg_connection *nc, int ev, void *ev_data, void *user_data) {

            MQTTManager *mqttManager = (MQTTManager *) user_data;


            switch (ev) {
                case MG_EV_CONNECT: {
                    LOG(LL_DEBUG, ("S6 Fresnel event: MG_EV_CONNECT => %d ", ev));
                    int connect_status = *(int *) ev_data;
                    if (connect_status != 0) {
                        LOG(LL_ERROR, ("Error: %d (%s)\r\n", connect_status, strerror(connect_status)));
                    } else {
                        LOG(LL_DEBUG, ("Got connected\r\n"));
                    }

                    struct mg_send_mqtt_handshake_opts opts;
                    memset(&opts, 0, sizeof(opts));

                    mg_set_protocol_mqtt(nc);
                    mg_send_mqtt_handshake_opt(nc, "S6 Fresnel", opts);
                }
                    break;

                case MG_EV_MQTT_CONNACK: {
                    struct mg_mqtt_message *msg = (struct mg_mqtt_message *) ev_data;
                    LOG(LL_DEBUG, ("S6 Fresnel event: MG_EV_MQTT_CONNACK => %d ", ev));
                    LOG(LL_DEBUG, ("S6 FRESNEL MG_EV_MQTT_CONNACK %d\n", msg->connack_ret_code));

                    if (msg->connack_ret_code != MG_EV_MQTT_CONNACK_ACCEPTED) {
                        if (mqttManager->callbacks[MqttEvent::ConnectError]) {
                            mqttManager->callbacks[MqttEvent::ConnectError]();
                        }
                        LOG(LL_DEBUG, ("Got mqtt connection error: %d\n", msg->connack_ret_code));
                    } else {
                        mqttManager->isConnected = true;
                        if (mqttManager->callbacks[MqttEvent::Connected]) {
                            mqttManager->callbacks[MqttEvent::Connected]();
                        }

                    }
                }
                    break;
                case MG_EV_MQTT_SUBSCRIBE:
                    LOG(LL_DEBUG, ("S6 Fresnel event: MG_EV_MQTT_SUBSCRIBE => %d ", ev));
                    if (mqttManager->callbacks[MqttEvent::Subscribe]) {
                        mqttManager->callbacks[MqttEvent::Subscribe]();
                    }
                    break;
                case MG_EV_MQTT_UNSUBSCRIBE:
                    LOG(LL_DEBUG, ("S6 Fresnel event: MG_EV_MQTT_UNSUBSCRIBE => %d ", ev));
                    break;
                    // "Trampoline" for handling subscribe
                case MG_EV_MQTT_PUBLISH:
                    LOG(LL_DEBUG, ("S6 Fresnel event: MG_EV_MQTT_PUBLISH => %d ", ev));
                    break;
                case MG_EV_MQTT_DISCONNECT:
                    LOG(LL_DEBUG, ("S6 Fresnel event: MG_EV_MQTT_DISCONNECT => %d ", ev));
                    mqttManager->isConnected = false;

                    break;

            }

            (void) nc;
        }

        static void mqtt_msg_handler(struct mg_connection *nc, const char *topic,
                                     int topic_len, const char *msg, int msg_len,
                                     void *ud) {
            (void)nc;
            SubscribeUserData *userData = (SubscribeUserData *) ud;
            userData->cb(topic, topic_len, msg, msg_len);
        }


        class SubscribeUserData {
        public:
            message_callback_t cb;
            MQTTManager *obj;
        };

        struct mg_connection *_mqttConn = NULL;
        std::map<MQTTManager::MqttEvent, event_callback_t> callbacks;


    public:

        MQTTManager() {
            mgos_mqtt_init();
            mgos_mqtt_add_global_handler(MQTTManager::mqtt_ev_handler, this);
        }


        void setEventCallback(MqttEvent event, event_callback_t cb) {
            callbacks[event] = cb;
        }

        void publish(std::string topic, std::string payload, int qos = 0, bool retain = false) {
            if (isConnected) {
                mgos_mqtt_pub(topic.c_str(),
                              reinterpret_cast<const void *>(payload.c_str()), payload.size(), qos, retain);
            } else {
                LOG(LL_DEBUG, ("Trying yo publish on %s with MQTT not connected", topic.c_str()));
            }
        }

        void subcribe(std::string topic, message_callback_t cb) {
            if (isConnected) {
                SubscribeUserData *ud = new SubscribeUserData();
                ud->cb = cb;
                ud->obj = this;
                mgos_mqtt_sub(topic.c_str(), mqtt_msg_handler, (void *) ud);

                LOG(LL_DEBUG, ("Subscribe to %s ", topic.c_str()));
            } else {
                LOG(LL_DEBUG, ("Trying yo subscribe on %s with MQTT not connected", topic.c_str()));
            }
        }
    };

}