#include <vector>
#include <iostream>

#include "../../../catch.hpp"

#include "../../../../src/hardware/devices/MCP39F511/MCP39F511DataTypes.hpp"


SCENARIO( "Manage uint16 data type", "[MCP39F511DataTypes]" ) {
    MCP39F511DataTypes dt;

    GIVEN( "A buffer containing a uint16 value" ) {
        uint8_t buffer[] = { 000, 0x00, 0x01, 0x02};

        WHEN("Decode with u16 method") {

            uint16_t res = dt.u16(buffer, 0);

            THEN("it should return 0x201"){
                REQUIRE(res == 0x201);
            }
        }
    }

    GIVEN( "A a uint16 value" ) {
        uint16_t value = 0x201;

        WHEN("Encode with w_u16 method") {
            uint8_t buffer[2];
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
        uint8_t buffer[] = { 0x00, 0x00, 0x01, 0x02, 0x03, 0x04 };

        WHEN("Decode with u32 method") {

            uint32_t res = dt.u32(buffer, 0);

            THEN("it should return 0x4030201"){
                REQUIRE(res == 0x4030201);
            }
        }
    }

    GIVEN( "A uint32 value" ) {
        uint32_t value = 0x4030201;

        WHEN("Encode with w_u32 method") {
            uint8_t buffer[4];
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

SCENARIO( "Manage uint64data type", "[MCP39F511DataTypes]" ) {
    MCP39F511DataTypes dt;

    GIVEN( "A buffer containing a uint64 value" ) {
        uint8_t buffer[] = { 0x00, 0x00, 0x01, 0x02, 0x03, 0x04,
                             0x05, 0x06, 0x07, 0x08 };

        WHEN("Decode with u64 method") {

            uint64_t res = dt.u64(buffer, 0);

            THEN("it should return 0x102030405060708"){
                REQUIRE(res == 0x807060504030201);
            }
        }
    }
}