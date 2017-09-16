//
// Created by Fabrizio Guglielmino on 16/09/17.
//

#pragma once

#include "../serializer/frozen.h"

/*
 Information about device and firmware

 example:
{
   "appName": "S6 Fresnel Module",
   "version": "1.0.15"
 */
char * devInfoMessage(const char *appName, const char *ver, const char *location, const char *name) {

    return NULL;
}


/*
 Device feedback after power switch request

example:
{
  "status": "on" // "on" | "off"
}
 */

char * powerFeedbackMessage(const char *status) {
    return NULL;
}

/*
 Immediate power consume message

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
 Last Willing Message (as supported by MQTT protocol)

 example:
{
  "status": "Offline"  // Online
}
 */
char *lwtMessage(const char *timestring, bool online) {
    return NULL;
}


