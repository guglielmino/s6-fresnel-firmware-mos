//
// Created by Fabrizio Guglielmino on 09/12/17.
//

#pragma once

#include "../../interfaces/IScalarSensor.h"

template<class T>
class SensorValue  {
private:
    T _value;
    bool _isValid;
public:
    SensorValue(T value, bool isValid = true) : _value(value), _isValid(isValid) {

    }

    T value() {
        return _value;
    }

    void setValue(T value, bool isValid = true) {
        _value = value;
        _isValid = isValid;
    }

    bool isValid() {
        return _isValid;
    }
};