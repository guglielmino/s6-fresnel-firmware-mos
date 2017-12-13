#!/bin/bash

if [ -z "$PLATFORM" ]
then
    echo "Set PLATFORM variable (esp32 or esp8266)"
    exit 1
fi

if [ -d "build/" ]
then
  rm -rf build/
fi

# CALIBRATION FIRMWARE
#mos build --platform=esp32 --cxxflags-extra -DS6TARGET=calibration

mos build --local --platform=$PLATFORM


if [ $? -eq 0 ]
then
  mv build/fw.zip build/s6fresnel-$PLATFORM.zip
else
  echo "Build failed"
fi

