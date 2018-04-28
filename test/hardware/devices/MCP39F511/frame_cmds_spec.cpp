#include <vector>
#include <iostream>

#include "../../../catch.hpp"

#include "../../../../src/hardware/devices/MCP39F511/MCP39F511Command.hpp"
#include "../../../../src/hardware/devices/MCP39F511/AddressPointerCmd.hpp"
#include "../../../../src/hardware/devices/MCP39F511/RegisterReadCmd.hpp"
#include "../../../../src/hardware/devices/MCP39F511/RegisterWriteCmd.hpp"
#include "../../../../src/hardware/devices/MCP39F511/AutoCalibrateGainCmd.hpp"
#include "../../../../src/hardware/devices/MCP39F511/AutoCalibrateReactiveGain.hpp"
#include "../../../../src/hardware/devices/MCP39F511/AutoCalibrateFrequencyCmd.hpp"
#include "../../../../src/hardware/devices/MCP39F511/SaveRegistersToFlashCmd.hpp"
#include "../../../../src/hardware/devices/MCP39F511/PageReadEEPROMCmd.hpp"
#include "../../../../src/hardware/devices/MCP39F511/PageWriteEEPROMCmd.hpp"
#include "../../../../src/hardware/devices/MCP39F511/BulkEraseEEPROMCmd.hpp"

#include "../../../../src/hardware/devices/MCP39F511/commands/ResetKWhAccumulator.hpp"
#include "../../../../src/hardware/devices/MCP39F511/commands/StartKWhAccumulator.hpp"

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

SCENARIO( "Save registers to Flash command", "[MCP39F511Commands]" ) {
    GIVEN( "We want to save registers state in Flash" ) {

        WHEN("create the command") {
            SaveRegistersToFlashCmd cmd;

            THEN("it should return the buffer") {
                std::vector<MCP39F511Command *> vec;
                vec.push_back(&cmd);
                std::vector<uint8_t> res = makeFrame(vec);
                REQUIRE(res[0] == 0xA5);
                REQUIRE(res[1] == 0x04);
                REQUIRE(res[2] == 0x53);

                REQUIRE(res.size() == 4);
            }
        }
    }
}

SCENARIO( "Page read EEPROM command", "[MCP39F511Commands]" ) {
    GIVEN( "We want to read a page #2 from EEPROM" ) {
        uint8_t  pageNum = 0x2;

        WHEN("create the command") {
            PageReadEEPROMCmd cmd(pageNum);

            THEN("it should return the buffer") {
                std::vector<MCP39F511Command *> vec;
                vec.push_back(&cmd);
                std::vector<uint8_t> res = makeFrame(vec);
                REQUIRE(res[0] == 0xA5);
                REQUIRE(res[1] == 0x05);
                REQUIRE(res[2] == 0x42);
                REQUIRE(res[3] == 0x02);

                REQUIRE(res.size() == 5);
            }
        }
    }
}

SCENARIO( "Page write EEPROM command", "[MCP39F511Commands]" ) {
    GIVEN( "We want to write 4bytes in EEPROM page #3" ) {
        std::vector<uint8_t> data = {0x03, 0x02, 0x01};
        uint8_t  pageNum = 0x3;

        WHEN("create the command") {
            PageWriteEEPROMCmd cmd(pageNum, data);

            THEN("it should return the buffer") {
                std::vector<MCP39F511Command *> vec;
                vec.push_back(&cmd);
                std::vector<uint8_t> res = makeFrame(vec);
                REQUIRE(res[0] == 0xA5);
                REQUIRE(res[1] == 0x08);
                REQUIRE(res[2] == 0x50);
                REQUIRE(res[3] == 0x03);
                REQUIRE(res[4] == 0x03);
                REQUIRE(res[5] == 0x02);
                REQUIRE(res[6] == 0x01);

                REQUIRE(res.size() == 8);
            }
        }
    }
}

SCENARIO( "Bulk erase EEPROM command", "[MCP39F511Commands]" ) {
    GIVEN( "We want to the bulk erase EEPROM command" ) {

        WHEN("create the command") {
            BulkEraseEEPROMCmd cmd;

            THEN("it should return the buffer") {
                std::vector<MCP39F511Command *> vec;
                vec.push_back(&cmd);
                std::vector<uint8_t> res = makeFrame(vec);
                REQUIRE(res[0] == 0xA5);
                REQUIRE(res[1] == 0x04);
                REQUIRE(res[2] == 0x4F);

                REQUIRE(res.size() == 4);
            }
        }
    }
}

SCENARIO( "Register Read command", "[MCP39F511Commands]" ) {
    GIVEN( "The number of bytes to read" ) {
        uint8_t nmBytes = 0x20;

        WHEN("create the command") {
            RegisterReadCmd cmd(nmBytes);

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
                REQUIRE(res[7] == 0x62);
                REQUIRE(res.size() == 8);
            }
        }
    }
}

SCENARIO( "Register Write command", "[MCP39F511Commands]" ) {
    GIVEN( "The data to write" ) {
        std::vector<uint8_t> data = {0x01, 0x02, 0x03};

        WHEN("create the command") {
            RegisterWriteCmd cmd(data);

            THEN("it should return the buffer") {
                std::vector<MCP39F511Command *> vec;
                vec.push_back(&cmd);
                std::vector<uint8_t> res = makeFrame(vec);
                REQUIRE(res[0] == 0xA5);
                REQUIRE(res[1] == 0x08);
                REQUIRE(res[2] == 0x4D);
                REQUIRE(res[3] == 0x03);
                REQUIRE(res[4] == 0x01);
                REQUIRE(res[5] == 0x02);
                REQUIRE(res[6] == 0x03);
                REQUIRE(res.size() == 8);
            }
        }
    }
}

SCENARIO( "Auto Calibrate Gain command", "[MCP39F511Commands]" ) {
    GIVEN( "We want to auto calibrate gain" ) {

        WHEN("create the command") {
            AutoCalibrateGainCmd cmd;

            THEN("it should return the buffer") {
                std::vector<MCP39F511Command *> vec;
                vec.push_back(&cmd);
                std::vector<uint8_t> res = makeFrame(vec);
                REQUIRE(res[0] == 0xA5);
                REQUIRE(res[1] == 0x04);
                REQUIRE(res[2] == 0x5A);

                REQUIRE(res.size() == 4);
            }
        }
    }
}

SCENARIO( "Auto Calibrate Reactive Gain command", "[MCP39F511Commands]" ) {
    GIVEN( "We want to auto calibrate gain" ) {

        WHEN("create the command") {
            AutoCalibrateReactiveGainCmd cmd;

            THEN("it should return the buffer") {
                std::vector<MCP39F511Command *> vec;
                vec.push_back(&cmd);
                std::vector<uint8_t> res = makeFrame(vec);
                REQUIRE(res[0] == 0xA5);
                REQUIRE(res[1] == 0x04);
                REQUIRE(res[2] == 0x7A);

                REQUIRE(res.size() == 4);
            }
        }
    }
}

SCENARIO( "Auto Calibrate Frequency Gain command", "[MCP39F511Commands]" ) {
    GIVEN( "We want to auto calibrate gain" ) {

        WHEN("create the command") {
            AutoCalibrateFrequencyCmd cmd;

            THEN("it should return the buffer") {
                std::vector<MCP39F511Command *> vec;
                vec.push_back(&cmd);
                std::vector<uint8_t> res = makeFrame(vec);
                REQUIRE(res[0] == 0xA5);
                REQUIRE(res[1] == 0x04);
                REQUIRE(res[2] == 0x76);

                REQUIRE(res.size() == 4);
            }
        }
    }
}

SCENARIO( "Application commands", "[StartKWhAccumulator]" ) {

    GIVEN("StartKWhAccumulator") {
        StartKWhAccumulator subject;

        WHEN("Getting the buffer") {
            std::vector<uint8_t> buffer = subject.frame();

            THEN("it should return start command buffer") {
                std::vector<uint8_t> expected = {0xa5, 0x0a, 0x41, 0x00, 0xdc, 0x4d, 0x02, 0x00, 0x80, 0x9b};
                REQUIRE(buffer == expected);
            }
        }
    }

    GIVEN("ResetKWhAccumulator") {
        ResetKWhAccumulator subject;

        WHEN("Getting the buffer") {
            std::vector<uint8_t> buffer = subject.frame();

            THEN("it should return reset command buffer") {
                std::vector<uint8_t> expected = {0xa5, 0x0a, 0x41, 0x00, 0xdc, 0x4d, 0x02, 0x00, 0x00, 0x1b};
                REQUIRE(buffer == expected);
            }
        }
    }
}