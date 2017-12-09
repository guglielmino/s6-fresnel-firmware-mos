#include <vector>
#include <iostream>

#include "../catch.hpp"

#include "../../src/hardware/sensors/SensorValue.hpp"


TEST_CASE("sensor value wrapper", "[SensorValue]") {

    SECTION("Create a valid sensor value (implicit)") {
        SensorValue<int> s(10);


        REQUIRE(s.value() == 10);
        REQUIRE(s.isValid());
    }

    SECTION("Create a valid sensor value") {
        SensorValue<float> s(13.5, true);

        REQUIRE(s.value() == 13.5);
        REQUIRE(s.isValid());
    }

    SECTION("Create an invalid value") {
        SensorValue<int> s(0, false);

        REQUIRE(s.value() == 0);
        REQUIRE(!s.isValid());
    }

    SECTION("Setting a value set it as valid implicitly") {
        SensorValue<int> s(0, false);

        s.setValue(3);

        REQUIRE(s.value() == 3);
        REQUIRE(s.isValid());
    }
}
