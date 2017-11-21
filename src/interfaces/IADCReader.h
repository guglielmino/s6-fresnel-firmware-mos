#pragma once

class IADCReader {
public:
    virtual int analogRead() = 0;
    virtual int resolution() = 0;
    virtual float maxVoltage() = 0;
    virtual ~IADCReader() = default;
};