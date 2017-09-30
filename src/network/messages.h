//
// Created by Fabrizio Guglielmino on 16/09/17.
//

#pragma once

#include "../libs/rapidjson/document.h"
#include "../libs/rapidjson/prettywriter.h" // for stringify JSON

#include "../serializer/frozen.h"

#include "mgos.h"

using namespace rapidjson;
using namespace std;

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
void devInfoMessage(char* infoMessage, const char *appName, const char *ver, const char *location, const char *name) {
   /*
    document["appName"].SetString(appName, static_cast<SizeType>(strlen(appName)), document.GetAllocator());
    document["version"].SetString(ver, static_cast<SizeType>(strlen(ver)), document.GetAllocator());
    document["location"].SetString(location, static_cast<SizeType>(strlen(location)), document.GetAllocator());
    document["name"].SetString(name, static_cast<SizeType>(strlen(name)), document.GetAllocator());
*/
    //StringBuffer sb;
    //PrettyWriter<StringBuffer> writer(sb);
   // document.Accept(writer);    // Accept() traverses the DOM and generates Handler events.
    //LOG(LL_DEBUG, ("%s", sb.GetString()));
   // strcpy(infoMessage, "TEST");

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


