#include <ctime>
#include "../catch.hpp"
#include "../../src/scheduler/Schedule.hpp"
#include "../../src/scheduler/Scheduler.hpp"



class ScheduleStub : public  IScheduledTask {
public:
    bool Execute(void *params) {
        INFO("EXECUTE");

        REQUIRE(true);
        return true;
    }
};

SCENARIO( "Scheduler run command based on scheduled rule ", "[Scheduler]" ) {

    GIVEN("A Scheduler with a task scheduled at 9:15pm") {
        INFO("RUN");
        Scheduler s;
        s.add(new ScheduleStub(), new Schedule(9, 15));

        WHEN("time is 9:15pm") {
            struct tm sampleTime = {
                    .tm_hour = 9,
                    .tm_min = 15,
                    .tm_sec = 0,
                    .tm_mday = 1,
                    .tm_mon = 1,
                    .tm_year = 117,
                    .tm_wday = 0,
                    .tm_yday = 1,
                    .tm_isdst = -1
            };
            time_t refTime = timegm(&sampleTime);

            THEN("should run scheduled task") {
                s.loop(&refTime);
            }
        }
    }
}
