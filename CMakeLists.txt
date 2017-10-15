cmake_minimum_required(VERSION 3.8)
project(s6_fresnel_firmware_mos)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        fs/index.html
        src/main.cpp
        src/interfaces/IADCReader.h
        src/interfaces/IScalarSensor.h
        src/hardware/S6FPowerSensor.h
        src/hardware/io/ESP32ADCReader.h
        mos.yml
        README.md src/network/mqtt-manager.h src/config/settings.h src/consts.h src/network/messages.h src/network/mqtt.h src/network/topics.h src/libs/frozen.c src/libs/frozen.h src/utils/dateutils.h src/hardware/gpio/OutputDevice.h src/hardware/io/ESP8266ADCReader.h src/hardware/SONOFFPowerSensor.h src/hardware/devices/xsns_hlw8012.h src/hardware/SONOFFDailyKwh.h src/hardware/S6DailyKwh.h src/hardware/gpio/InputDevice.h src/hardware/S6ExtPowerSensor.h src/devfunctions.h src/globals.h src/interfaces/ISPI.h src/hardware/serials/SPIManager.h)

add_executable(s6_fresnel_firmware_mos ${SOURCE_FILES})