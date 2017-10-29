#include <vector>
#include <iostream>

#include "../../../catch.hpp"
#include "hardware/devices/MCP39F511/MCP39F511Command.hpp"
#include "hardware/devices/MCP39F511/AddressPointerCmd.hpp"
#include "hardware/devices/MCP39F511/RegisterReadCmd.hpp"

SCENARIO( "Set Address Pointer command", "[MCP39F511Commands]" ) {
    GIVEN( "A register address split in high and low byte" ) {
        uint8_t high = 00;
        uint8_t low = 06;

        WHEN("create the command") {
            AddressPointerCmd cmd(high, low);

            THEN("it should return the buffer") {
                std::vector<MCP39F511Command *> vec;
                vec.push_back(&cmd);
                std::vector<uint8_t> res = makeFrame(vec);
                REQUIRE(res[0] == 0xA5);
                REQUIRE(res[1] == 0x06);
                REQUIRE(res[2] == 0x41);
                REQUIRE(res[3] == 0x00);
                REQUIRE(res[4] == 0x06);
                REQUIRE(res.size() == 6);

            }
        }
    }
}

SCENARIO( "Register Read command", "[MCP39F511Commands]" ) {
    GIVEN( "The number of bytes to read" ) {
        uint8_t nmBytes = 00;

        WHEN("create the command") {
            RegisterReadCmd cmd(0x20);

            THEN("it should return the buffer") {
                std::vector<MCP39F511Command *> vec;
                vec.push_back(&cmd);
                std::vector<uint8_t> res = makeFrame(vec);
                REQUIRE(res[0] == 0xA5);
                REQUIRE(res[1] == 0x05);
                REQUIRE(res[2] == 0x4E);
                REQUIRE(res[3] == 0x20);
                REQUIRE(res.size() == 5);
            }
        }
    }
}

SCENARIO( "Set Address Pointer and Register Read composed command", "[MCP39F511Commands]" ) {
    GIVEN( "The number of bytes to read" ) {
        uint8_t nmBytes = 0x20;
        uint8_t high = 00;
        uint8_t low = 06;


        WHEN("create the command") {
            AddressPointerCmd cmd1(high, low);
            RegisterReadCmd cmd2(nmBytes);

            THEN("it should return the buffer") {
                std::vector<MCP39F511Command *> vec;
                vec.push_back(&cmd1);
                vec.push_back(&cmd2);
                std::vector<uint8_t> res = makeFrame(vec);
                REQUIRE(res[0] == 0xA5);
                REQUIRE(res[1] == 0x08);
                REQUIRE(res[2] == 0x41);
                REQUIRE(res[3] == 0x00);
                REQUIRE(res[4] == 0x06);
                REQUIRE(res[5] == 0x4E);
                REQUIRE(res[6] == 0x20);
                REQUIRE(res.size() == 8);
            }
        }
    }
}