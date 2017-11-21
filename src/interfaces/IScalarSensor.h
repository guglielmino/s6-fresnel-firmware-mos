/**
 * Interface for reading scalar value from sensors
 */

#pragma once

template<class T> 
class IScalarSensor {
  public:
    virtual T readValue() = 0;
    virtual ~IScalarSensor() = default;
};

