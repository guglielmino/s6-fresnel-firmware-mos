cmake_minimum_required(VERSION 3.8)
project(s6_fresnel_firmware_mos)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        fs/index.html
        src/main.cpp
        src/IADCReader.h
        src/IScalarSensor.h
        src/hardware/PowerSensor.h
        src/hardware/io/ESP32ADCReader.h
        mos.yml
        README.md src/network/mqtt-manager.h src/config/settings.h src/consts.h src/version.h src/network/messages.h src/network/mqtt.h src/network/topics.h)

add_executable(s6_fresnel_firmware_mos ${SOURCE_FILES})