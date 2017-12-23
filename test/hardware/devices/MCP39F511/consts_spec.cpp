#include "../../../catch.hpp"
#include "../../../../src/hardware/devices/MCP39F511/MCP39F511Consts.h"

TEST_CASE("Values conversion table", "[Conversions]") {

    SECTION("Current conversion") {
        float res = REG_CURRENT.rawToValue(10000);
        REQUIRE(res == 1.0f);
    }  
}
