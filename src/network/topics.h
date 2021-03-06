//
// Created by Fabrizio Guglielmino on 16/09/17.
//

#pragma once

/* --- MQTT publish topics -- */
#define PUB_SENS_INFO_TOPIC         "%s/%s/events/%s/info"            // Device publish info message on this topic
#define PUB_SENS_POWER_TOPIC        "%s/%s/sensors/%s/power"          // Device publish power consume on this topic
#define PUB_SENS_RPOWER_TOPIC       "%s/%s/sensors/%s/reactivepower"  // Device publish Ractive Power Message on this topic

#define PUB_SENS_DAILYKWH_TOPIC     "%s/%s/sensors/%s/dailyKwh"       // Device publish daily Kw/h accumulated consume on this topic
#define PUB_SENS_CURRENT_TOPIC      "%s/%s/sensors/%s/current"        // Device publish Current RMS Message on this topic
#define PUB_SENS_FREQUENCY_TOPIC    "%s/%s/sensors/%s/frequency"      // Device publish Line Frequency Message on this topic
#define PUB_SENS_POWERFACTOR_TOPIC  "%s/%s/sensors/%s/powerfactor"    // Device publish Power Factor Message on this topic
#define PUB_SENS_VOLTAGE_TOPIC      "%s/%s/sensors/%s/voltage"        // Device publishVoltage Message on this topic


#define PUB_EVENT_POWERFEEDBACK_TOPIC   "%s/%s/events/%s/power"    // Device publish power switch feedback consume on this topic
#define PUB_EVENT_LWT_TOPIC         "%s/%s/events/%s/lwt"          // Device publish Last Willing Message on this topic
#define PUB_EVENT_CRONTAB_TOPIC     "%s/%s/events/%s/crontab"      // Device publish current crontab on this topic


/* --- MQTT subsribe topics ---*/
#define SUB_SWITCH_DEV         "%s/%s/devices/%s/power"    // Device subscribe to this topic to handle power switch command (by device)
#define SUB_SWITCH_ROOM        "%s/%s/commands/power"      // Device subscribe to this topic to handle power switch command (by room)
#define SUB_UPGRADE_DEV        "%s/%s/devices/%s/upgrade"  // Device subscribe to this topic to handle firmware upgrade command (by device)
#define SUB_UPGRADE_ROOM       "%s/%s/commands/upgrade"    // Device subscribe to this topic to handle firmware upgrade command (by room)


// Composed topics
#define MAX_TOPIC_LEN             60
char pubSensInfoTopic[MAX_TOPIC_LEN];
char pubSensPowerTopic[MAX_TOPIC_LEN];
char pubSensReactivePowerTopic[MAX_TOPIC_LEN];
char pubSensDailyKwhTopic[MAX_TOPIC_LEN];
char pubEventPowerTopic[MAX_TOPIC_LEN];
char pubInfoTopic[MAX_TOPIC_LEN];
char pubPowerFeedbackTopic[MAX_TOPIC_LEN];
char pubLWTTopic[MAX_TOPIC_LEN];
char pubSensCurrentTopic[MAX_TOPIC_LEN];
char pubSensFreqTopic[MAX_TOPIC_LEN];
char pubSensPowerFactorTopic[MAX_TOPIC_LEN];
char pubSensVoltageTopic[MAX_TOPIC_LEN];
char pubEventCrontabTopic[MAX_TOPIC_LEN];

char subSwitchDevTopic[MAX_TOPIC_LEN];
char subSwitchRoomTopic[MAX_TOPIC_LEN];

void makeDeviceTopic(char *topic, int topicLen, const char *topicPattern, const char *gateway, const char *room, const char *deviceId) {
    memset (topic, 0, topicLen);
    snprintf(topic, topicLen, topicPattern, gateway, room, deviceId);
}

void makeRoomTopic(char * topic, int topicLen, const char *topicPattern, const char *gateway, const char *room) {
    memset (topic, 0, topicLen);
    snprintf(topic, topicLen, topicPattern, gateway, room);
}