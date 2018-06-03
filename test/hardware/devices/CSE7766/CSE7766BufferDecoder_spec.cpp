//
// Created by Fabrizio Guglielmino on 02/06/18.
//

#include "../../../catch.hpp"
#include "../../../../src/hardware/devices/CSE7766/CSE7766BufferDecoder.hpp"

// Sample data:
// 55 5A 02 E9 50 00 03 31 00 3E 9E 00 0D 30 4F 44 F8 00 12 65 F1 81 76 72 (w/ load)
// F2 5A 02 E9 50 00 03 2B 00 3E 9E 02 D7 7C 4F 44 F8 CF A5 5D E1 B3 2A B4 (w/o load)


bool fequal(double a, double b) {
    return fabs(a - b) < 0.0005;
}

SCENARIO("Decode CSE7766 buffers", "[CSE7766BufferDecoder]") {
    CSE7766BufferDecoder vd;

    GIVEN("A buffer with load ") {
        uint8_t buffer[] = {0x55, 0x5A, 0x02, 0xE9, 0x50, 0x00, 0x03, 0x31,
                            0x00, 0x3E, 0x9E, 0x00, 0x0D, 0x30, 0x4F, 0x44,
                            0xF8, 0x00, 0x12, 0x65, 0xF1, 0x81, 0x76, 0x72};

        WHEN("process the buffer") {
            CSE7766BufferDecoder::ProcessResult res = vd.process(buffer);

            THEN("it should return energy values") {
                REQUIRE(res.error == CSE7766BufferDecoder::ErrorCodes::SUCCESS);
                REQUIRE(fequal(res.v.voltage, 233.537332));
                REQUIRE(fequal(res.v.activePower, 1103.206626));
                REQUIRE(fequal(res.v.current, 4.748223));
                REQUIRE(fequal(res.v.energy, 4.748223));
            }
        }
    }

    GIVEN("A buffer without load ") {
        uint8_t buffer[] = {0xF2, 0x5A, 0x02, 0xE9, 0x50, 0x00, 0x03, 0x2B,
                            0x00, 0x3E, 0x9E, 0x02, 0xD7, 0x7C, 0x4F, 0x44,
                            0xF8, 0xCF, 0xA5, 0x5D, 0xE1, 0xB3, 0x2A, 0xB4};

        WHEN("process the buffer") {
            CSE7766BufferDecoder::ProcessResult res = vd.process(buffer);

            THEN("it should return energy values") {
                REQUIRE(res.error == CSE7766BufferDecoder::ErrorCodes::SUCCESS);
                REQUIRE(fequal(res.v.voltage, 235.265105));
                REQUIRE(fequal(res.v.activePower, 0));
                REQUIRE(fequal(res.v.current, 0.086074));
                REQUIRE(fequal(res.v.energy, 66101.264442));
            }
        }
    }


}