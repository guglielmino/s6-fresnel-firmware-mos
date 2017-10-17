#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <ctime>

#include "../catch.hpp"

#include "../../src/scheduler/Schedule.hpp"

SCENARIO( "Schedule tell if time match running specification", "[Schedule]" ) {

    GIVEN( "A Schedule for 6:25pm" ) {
        Schedule s(6, 25);

        WHEN( "time is 6:25pm" ) {
            struct tm sampleTime = {
                    .tm_hour = 6,
                    .tm_min = 25,
                    .tm_sec = 0,
                    .tm_mday = 1,
                    .tm_mon = 1,
                    .tm_year = 117,
                    .tm_wday = 0,
                    .tm_yday = 1,
                    .tm_isdst = -1
            };
            time_t refTime = timegm(&sampleTime);

            THEN( "it should run" ) {
                REQUIRE(s.ShouldRun(&refTime) == true);
            }
        }

        WHEN( "time is 9:40pm" ) {
            struct tm sampleTime = {
                    .tm_hour = 9,
                    .tm_min = 40,
                    .tm_sec = 0,
                    .tm_mday = 1,
                    .tm_mon = 1,
                    .tm_year = 117,
                    .tm_wday = 0,
                    .tm_yday = 1,
                    .tm_isdst = -1
            };
            time_t refTime = timegm(&sampleTime);

            THEN( "it shouldn't run" ) {
                REQUIRE(s.ShouldRun(&refTime) == false);
            }
        }
    }
}