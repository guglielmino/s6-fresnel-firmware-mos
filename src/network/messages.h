//
// Created by Fabrizio Guglielmino on 16/09/17.
//

#pragma once

//#include "../libs/rapidjson/document.h"
//#include "../libs/rapidjson/prettywriter.h" // for stringify JSON

#include "../libs/frozen.h"

#include "mgos.h"

//using namespace rapidjson;
//using namespace std;

//static Document document;

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
void devInfoMessage(char *buffer, size_t len, const char *appName, const char *ver, const char *location, const char *name) {
    struct json_out out = JSON_OUT_BUF(buffer, len);
    json_printf(&out, "{ %Q: %Q, %Q: %Q, %Q: %Q, %Q: %Q }", "appName", appName, "version", ver,
                "location", location, "name", name);
}



/*
 Immediate power consume message (W)

 example:
{
  "timestamp": "2017-07-08T12:47:36",
  “power": 23.3
}
 */
void powerConsumeMessage(char *buffer, size_t len, const char *timestring, float value) {
    struct json_out out = JSON_OUT_BUF(buffer, len);
    json_printf(&out, "{ %Q: %Q, %Q: %f }", "timestamp", timestring, "power", value);
}

/*
 Accumulated daily consume (Kw/h)

 example:
{
  "timestamp": "2017-07-08T12:47:36",
  “consume": 23.3
}
 */
void dailyConsumeMessage(char *buffer, size_t len, const char *timestring, float value) {
    struct json_out out = JSON_OUT_BUF(buffer, len);
    json_printf(&out, "{ %Q: %Q, %Q: %f }", "timestamp", timestring, "consume", value);
}

/*
 Feedback message after Relay switch

{
    "status": "on" // "on" | "off"
}


 */
void powerFeedbackMessage(char *buffer, size_t len, bool on) {
    struct json_out out = JSON_OUT_BUF(buffer, len);
    json_printf(&out, "{ %Q: %Q }", "status", (on ? "on" : "off"));
}

/*
 Last Willing Message (as supported by MQTT protocol)

 example:
{
  "timestamp": "2017-07-08T12:47:36",
  "status": "Offline" // “Online”
}

 */
void lwtMessage(char *buffer, size_t len, bool online) {
    struct json_out out = JSON_OUT_BUF(buffer, len);
    json_printf(&out, "{ %Q: %Q }", "status", (online ? "Online" : "Offline"));
}


