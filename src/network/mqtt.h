//
// Created by Fabrizio Guglielmino on 16/09/17.
//

#pragma once


#include <map>
#include "mgos.h"
#include "mgos_mqtt.h"


class MQTTManager {
public:
    enum MqttEvent {
        NoOp, Connected
    };

    typedef void (*message_callback_t)(const char *topic, size_t topic_len, const char *msg, size_t msg_len);

private:
    typedef void (*event_callback_t)(void);

    bool isConnected = false;

    static void mqtt_msg_handler(struct mg_connection *nc, const char *topic,
                                 int topic_len, const char *msg, int msg_len,
                                 void *ud) {
        (void) nc;
        SubscribeUserData *userData = (SubscribeUserData *) ud;
        userData->cb(topic, topic_len, msg, msg_len);
    }

    static void mqtt_connect_handler(struct mg_connection *nc,
                                     const char *client_id,
                                     struct mg_send_mqtt_handshake_opts *opts,
                                     void *fn_arg) {
        MQTTManager *mqttManager = (MQTTManager *) fn_arg;

        mg_set_protocol_mqtt(nc);
        mg_send_mqtt_handshake_opt(nc, "S6 Fresnel", *opts);


        mqttManager->isConnected = true;
        if (mqttManager->callbacks[MqttEvent::Connected]) {
            mqttManager->callbacks[MqttEvent::Connected]();
        }
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
        mgos_mqtt_set_connect_fn(mqtt_connect_handler, this);
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

