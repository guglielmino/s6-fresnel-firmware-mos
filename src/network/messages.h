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
   "appName": "S6 Fresnel Module",
   "version": "1.0.15",
   "group": "room1",
   "name": "lamp1",
   "features: ["POWERMETER", "RELAY1", "RELAY2"]
}
 */
std::string devInfoMessage(const char *appName, const char *ver, const char *group, const char *name,
                           std::vector<const char *> features) {
    char buffer[MESSAGE_BUFFER_LEN] = "";

    struct json_out out = JSON_OUT_BUF(buffer, MESSAGE_BUFFER_LEN);
    json_printf(&out, "{ appName: %Q, version: %Q, group: %Q, name: %Q, features: %M }",  appName, ver, group, name,
                json_printf_array, features.data(), features.size() * sizeof(features[0]), sizeof(features[0]), "%Q");
    return std::string(buffer);
}


/*
 Feedback message after Relay switch

{
    "status": "on" // "on" | "off",
    "relay_idx": 0
}

 */
std::string powerFeedbackMessage(bool on, int relayIdx) {
    char buffer[MESSAGE_BUFFER_LEN] = "";
    struct json_out out = JSON_OUT_BUF(buffer, MESSAGE_BUFFER_LEN);
    json_printf(&out, "{ status: %Q, relay_idx: %d }",  (on ? "on" : "off"), relayIdx);
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
std::string lwtMessage(bool online) {
    char buffer[MESSAGE_BUFFER_LEN] = "";
    struct json_out out = JSON_OUT_BUF(buffer, MESSAGE_BUFFER_LEN);
    json_printf(&out, "{ status: %Q }", (online ? "Online" : "Offline"));
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