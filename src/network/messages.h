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
   "location": "room1",
   "name": "lamp1"
}
 */
std::string devInfoMessage(const char *appName, const char *ver, const char *location, const char *name) {
    char buffer[MESSAGE_BUFFER_LEN] = "";
    struct json_out out = JSON_OUT_BUF(buffer, MESSAGE_BUFFER_LEN);
    json_printf(&out, "{ %Q: %Q, %Q: %Q, %Q: %Q, %Q: %Q }", "appName", appName, "version", ver,
                "location", location, "name", name);
    return std::string(buffer);
}



/*
 Immediate power consume message (W)

 example:
{
  "timestamp": "2017-07-08T12:47:36",
  “value": 23.3,
  "unit": "W"
}
 */
std::string powerConsumeMessage(const char *timestring, float value) {
    char buffer[MESSAGE_BUFFER_LEN] = "";
    struct json_out out = JSON_OUT_BUF(buffer, MESSAGE_BUFFER_LEN);
    json_printf(&out, "{ %Q: %Q, %Q: %.6f, %Q: %Q  }", "timestamp", timestring, "value", value,  "unit", "W");
    return std::string(buffer);
}

/*
 Accumulated daily consume (Kw/h)

 example:
{
  "timestamp": "2017-07-08T12:47:36",
  “value": 23.3,
  "unit": "KWh"
}
 */
std::string dailyConsumeMessage(const char *timestring, float value) {

    char buffer[MESSAGE_BUFFER_LEN] = "";
    struct json_out out = JSON_OUT_BUF(buffer, MESSAGE_BUFFER_LEN);
    json_printf(&out, "{ %Q: %Q, %Q: %.6f, %Q: %Q }", "timestamp", timestring, "value", value, "unit", "KWh");
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
 current RMS (A)

 example:
{
  "timestamp": "2017-07-08T12:47:36",
  “value": 23.3,
  "unit": "A"
}
 */
std::string currentRMSMessage(const char *timestring, float value) {

    LOG(LL_DEBUG, ("*** CURRENT  %.6f", value));
    char buffer[MESSAGE_BUFFER_LEN] = "";
    struct json_out out = JSON_OUT_BUF(buffer, MESSAGE_BUFFER_LEN);
    json_printf(&out, "{ %Q: %Q, %Q: %.6f, %Q: %Q }", "timestamp", timestring, "value", value, "unit", "A");
    return std::string(buffer);
}