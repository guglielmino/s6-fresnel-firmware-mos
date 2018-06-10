#!/bin/bash

if [ -z "$1" ]
then
    echo "Use $0 PLATFORM (esp32 or esp8266) DEVICE (DEV_FRESNEL or DEV_POWR2)"
    exit 1
fi

PLATFORM=$1

if [ -d "build/" ]
then
  rm -rf build/
fi

mos build --local --verbose --no-libs-update --libs-dir ../mos-libs/  --platform=$PLATFORM --cxxflags-extra -D$DEVICE

if [ $? -eq 0 ]
then
  cp build/fw.zip build/s6fresnel-$PLATFORM.zip
else
  echo "Build failed"
fi

