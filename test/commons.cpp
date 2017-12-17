#include "catch.hpp"

#include "../src/hardware/sensors/SensorValue.hpp"


SensorValue<int> fun() {
    SensorValue<int> a(10);
    return a;
}

TEST_CASE("memory handling tests", "[generic]") {

    SECTION("returned value should be valid") {
        SensorValue<int> r = fun();
        REQUIRE(r.value() == 10);
    }

}