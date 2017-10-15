//
// Created by Fabrizio Guglielmino on 16/09/17.
//

#pragma once

#define FIRMWARE_APP_VERSION "0.0.5"

#define MQTT_MESSAGE_SIZE       60

#if CS_PLATFORM == CS_P_ESP8266
#define FIRMWARE_APP_NAME "S6 Legacy Pow Module"
#define REL_PIN         15
#define STATUS_LED_PIN  12
#define BUTTON_PIN       0
#elif CS_PLATFORM == CS_P_ESP32
#define FIRMWARE_APP_NAME "S6 Fresnel Module"
#define REL_PIN         32
#define STATUS_LED_PIN  33
#define BUTTON_PIN       0
#endif

