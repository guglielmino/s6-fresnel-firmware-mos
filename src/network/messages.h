//
// Created by Fabrizio Guglielmino on 16/09/17.
//

#pragma once

#include "../libs/frozen.h"

#include "mgos.h"

#define MESSAGE_BUFFER_LEN 100

/*
 Information about device and firmware

 example:
{
   "appName": "S6 Fresnel Module",
   "version": "1.0.15",
   "group": "room1",
   "name": "lamp1"
}
 */
std::string devInfoMessage(const char *appName, const char *ver, const char *group, const char *name) {
    char buffer[MESSAGE_BUFFER_LEN] = "";
    struct json_out out = JSON_OUT_BUF(buffer, MESSAGE_BUFFER_LEN);
    json_printf(&out, "{ %Q: %Q, %Q: %Q, %Q: %Q, %Q: %Q }", "appName", appName, "version", ver,
                "group", group, "name", name);
    return std::string(buffer);
}

/*
 Feedback message after Relay switch

{
    "status": "on" // "on" | "off"
}

 */
std::string powerFeedbackMessage(bool on) {
    char buffer[MESSAGE_BUFFER_LEN] = "";
    struct json_out out = JSON_OUT_BUF(buffer, MESSAGE_BUFFER_LEN);
    json_printf(&out, "{ %Q: %Q }", "status", (on ? "on" : "off"));
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
    json_printf(&out, "{ %Q: %Q }", "status", (online ? "Online" : "Offline"));
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
    json_printf(&out, "{ %Q: %Q, %Q: %.6f, %Q: %Q }", "timestamp", timestring, "value", value, "unit", unit);
    return std::string(buffer);
}