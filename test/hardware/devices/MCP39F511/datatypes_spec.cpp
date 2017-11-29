#include <vector>
#include <iostream>

#include "../../../catch.hpp"

#include "../../src/hardware/devices/MCP39F511/MCP39F511DataTypes.hpp"


SCENARIO( "Manage uint16 data type", "[MCP39F511DataTypes]" ) {
    MCP39F511DataTypes dt;

    GIVEN( "A buffer containing a uint16 value" ) {
        char buffer[] = { 000, 0x00, 0x01, 0x02};

        WHEN("Decode with u16 method") {

            uint16_t res = dt.u16(buffer, 0);

            THEN("it should return 513"){
                REQUIRE(res == 513);
            }
        }
    }

    GIVEN( "A a uint16 value" ) {
        uint16_t value = 513;

        WHEN("Encode with w_u16 method") {
            char buffer[2];
            dt.w_u16(value, buffer);

            THEN("it should return 0x01 0x02"){
                REQUIRE(buffer[0] == 0x01);
                REQUIRE(buffer[1] == 0x02);
            }
        }
    }
}

SCENARIO( "Manage uint32 data type", "[MCP39F511DataTypes]" ) {
    MCP39F511DataTypes dt;

    GIVEN( "A buffer containing a uint32 value" ) {
        char buffer[] = { 000, 0x00, 0x01, 0x02, 0x03, 0x04};

        WHEN("Decode with u16 method") {

            uint32_t res = dt.u32(buffer, 0);

            THEN("it should return 67305985"){
                REQUIRE(res == 67305985);
            }
        }
    }

    GIVEN( "A uint32 value" ) {
        uint32_t value = 67305985;

        WHEN("Encode with w_u32 method") {
            char buffer[4];
            dt.w_u32(value, buffer);

            THEN("it should return 0x01 0x02 0x03 0x04"){
                REQUIRE(buffer[0] == 0x01);
                REQUIRE(buffer[1] == 0x02);
                REQUIRE(buffer[2] == 0x03);
                REQUIRE(buffer[3] == 0x04);
            }
        }
    }
}