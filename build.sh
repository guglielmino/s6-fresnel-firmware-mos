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

mos build --platform=$PLATFORM

if [ $? -eq 0 ]
then
  mv build/fw.zip build/s6fresnel-$PLATFORM.zip
else
  echo "Build failed"
fi

