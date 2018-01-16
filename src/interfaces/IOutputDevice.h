//
// Created by Fabrizio Guglielmino on 16/01/18.
//

#pragma once

typedef enum {
    ON,
    OFF
} SwitchMode;

class IOutputDevice {
public:
    virtual void turn(SwitchMode mode) = 0;
    virtual void toggle() = 0;
    virtual ~IOutputDevice() = default;
};