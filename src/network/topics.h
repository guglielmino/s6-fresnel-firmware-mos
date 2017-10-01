//
// Created by Fabrizio Guglielmino on 16/09/17.
//

#pragma once

/* --- MQTT publish topics -- */
#define PUB_SENS_INFO_TOPIC     "building/%s/sensors/%s/info"        // Device publish info message on this topic
#define PUB_SENS_POWER_TOPIC    "building/%s/sensors/%s/power"       // Device publish power consume on this topic
#define PUB_SENS_DAILYKWH_TOPIC "building/%s/sensors/%s/dailyKwh"    // Device publish daily Kw/h accumulated consume on this topic
#define PUB_EVENT_POWER_TOPIC   "building/%s/events/%s/power"        // Device publish power switch feedback consume on this topic


/* --- MQTT subsribe topics ---*/
#define SUB_SWITCH_DEV         "building/%s/devices/%s/power"    // Device subscribe to this topic to handle power switch command (by device)
#define SUB_SWITCH_ROOM        "building/%s/commands/power"      // Device subscribe to this topic to handle power switch command (by room)
#define SUB_UPGRADE_DEV        "building/%s/devices/%s/upgrade"  // Device subscribe to this topic to handle firmware upgrade command (by device)
#define SUB_UPGRADE_ROOM       "building/%s/commands/upgrade"    // Device subscribe to this topic to handle firmware upgrade command (by room)


// Composed topics
#define MAX_TOPIC_LEN             60
char pubSensInfoTopic[MAX_TOPIC_LEN];
char pubSensPowerTopic[MAX_TOPIC_LEN];
char pubSensDailyKwhTopic[MAX_TOPIC_LEN];
char pubEventPowerTopic[MAX_TOPIC_LEN];
char pubInfoTopic[MAX_TOPIC_LEN];

char subSwitchDevTopic[MAX_TOPIC_LEN];
char subSwitchRoomTopic[MAX_TOPIC_LEN];


void makeDeviceTopic(char *topic, int topicLen, const char *topicPattern, const char *room, const char *deviceId) {
    memset (topic, 0, topicLen);
    snprintf(topic, topicLen, topicPattern, room, deviceId);
}

void makeRoomTopic(char * topic, int topicLen, const char *topicPattern, const char *room) {
    memset (topic, 0, topicLen);
    snprintf(topic, topicLen, topicPattern, room);
}