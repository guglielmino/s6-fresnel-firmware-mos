# Firmware sources

## Overview



## How to install this app on the device

- Install [mos tool](https://mongoose-os.com/software.html)
- run `mos build --platform=esp32` to build the firmware for ESP32 platform
- run `mos flash --port=/dev/tt{portname}` to flash the firmware on the device (port name have to be checked)
- run `mos console --port=/dev/tty{portname}` to read device log in the terminal


## How to run test

- run cmake . (on root folder)
- run make
- run ./test/s6test

## References

- Mongoose OS repo    : https://github.com/cesanta/mongoose-os
- Libs                : https://github.com/mongoose-os-libs
- Example apps        : https://github.com/mongoose-os-apps

## Refactoring notes

- Generic MCP39F511 classes for reading u16, u32 and u64 registers
- Remove UART singleton from sensor factories source file