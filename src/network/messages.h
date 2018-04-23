//
// Created by Fabrizio Guglielmino on 16/09/17.
//

#pragma once


#include <string>
#include <vector>
#include "../libs/frozen.h"



#define MESSAGE_BUFFER_LEN 400

/*
 Information about device and firmware

 example:
{
   "timestamp": "2017-07-08T12:47:36",
   "appName": "S6 Fresnel Module",
   "version": "1.0.15",
   "group": "room1",
   "name": "lamp1",
   "wifi": {ip: "192.168.0.1"},
   "eth": {ip: ""},
   "features: ["POWERMETER", "RELAY1", "RELAY2"]
}
 */
std::string devInfoMessage(const char *timestring, const char *appName, const char *ver, const char *group, const char *name,
                           std::string wifiIp, std::string ethIp, std::vector<std::string> features) {
    char buffer[MESSAGE_BUFFER_LEN] = "";

    struct json_out out = JSON_OUT_BUF(buffer, MESSAGE_BUFFER_LEN);
    json_printf(&out, "{ timestamp: %Q, appName: %Q, version: %Q, group: %Q, name: %Q, wifi: {ip: %Q}, eth: {ip: %Q}, features: %M }",
                timestring, appName, ver, group, name, wifiIp.c_str(), ethIp.c_str(),
                json_printf_array, features.data(), features.size() * sizeof(features[0]), sizeof(features[0]), "%Q");
    return std::string(buffer);
}


/*
 Feedback message after Relay switch

{
    "timestamp": "2017-07-08T12:47:36",
    "status": "on" // "on" | "off",
    "relay_idx": 0
}

 */
std::string powerFeedbackMessage(const char *timestring, bool on, int relayIdx) {
    char buffer[MESSAGE_BUFFER_LEN] = "";
    struct json_out out = JSON_OUT_BUF(buffer, MESSAGE_BUFFER_LEN);
    json_printf(&out, "{ timestamp: %Q, status: %Q, relay_idx: %d }", timestring, (on ? "on" : "off"), relayIdx);
    return std::string(buffer);
}

/*
 Last Willing Message (as supported by MQTT protocol)

 example:
{
  "timestamp": "2017-07-08T12:47:36",
  "status": "Offline" // “Online”
}

 */
std::string lwtMessage(const char *timestring, bool online) {
    char buffer[MESSAGE_BUFFER_LEN] = "";
    struct json_out out = JSON_OUT_BUF(buffer, MESSAGE_BUFFER_LEN);
    json_printf(&out, "{ timestamp: %Q, status: %Q }", timestring, (online ? "Online" : "Offline"));
    return std::string(buffer);
}

/*

 example:
{
  "timestamp": "2017-07-08T12:47:36",
  “value": 50.3,
  "unit": "Hz"
}
 */
std::string  makeSensorValueMessage(const char *timestring, float value, const char * unit) {
    char buffer[MESSAGE_BUFFER_LEN] = "";
    struct json_out out = JSON_OUT_BUF(buffer, MESSAGE_BUFFER_LEN);
    json_printf(&out, "{ timestamp: %Q, value: %.6f, unit: %Q }", timestring, value, unit);
    return std::string(buffer);
}