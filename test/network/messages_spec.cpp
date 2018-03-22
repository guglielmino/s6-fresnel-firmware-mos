#include <cstring>
#include <iostream>
#include <vector>
#include "../catch.hpp"
#include "../../src/libs/frozen.h"
#include "../../src/network/messages.h"

static void scan_array(const char *str, int len, void *user_data) {
    struct json_token t;
    int i;
    printf("Parsing array: %.*s  \n", len, str);
    for (i = 0; json_scanf_array_elem(str, len, ".features", i, &t) > 0; i++) {
        printf("Index %d, token [%.*s]\n", i, t.len, t.ptr);
    }
}

TEST_CASE("messages ", "[messages]") {

    SECTION("device info message") {
        std::vector<const char *> arr = { "POWERMETER", "RELAY1", "RELAY2"};

        std::string res = devInfoMessage("Sample", "1.0.0", "a group", "a name", arr);
       // std::cout << res << "\n\n";

        char *appName, *version, *group, *name;
        json_scanf(res.c_str(), res.length(), "{ appName: %Q, version: %Q, group: %Q, name: %Q, features: %M }",
                   &appName, &version, &group, &name, scan_array);

        REQUIRE(res.length() > 0);
        REQUIRE(strcmp(appName, "Sample") == 0);
        REQUIRE(strcmp(version, "1.0.0") == 0);
        REQUIRE(strcmp(group, "a group") == 0);
        REQUIRE(strcmp(name, "a name") == 0);
    }

    SECTION("power feedback message") {
        std::string res = powerFeedbackMessage(true, 1);

        char *status;
        int idx;
        json_scanf(res.c_str(), res.length(), "{ status: %Q, relay_idx: %d }",
                   &status, &idx);

        REQUIRE(res.length() > 0);
        REQUIRE(strcmp(status, "on") == 0);
        REQUIRE(idx == 1);
    }




    SECTION("sensor value message") {
        std::string res = makeSensorValueMessage("2017-07-08T12:47:36", 43.00f, "W");

        char *timestamp, *unit;
        float value = 0.0;
        int result = json_scanf(res.c_str(), res.length(), "{ timestamp: %Q, value: %f, unit: %Q }",
                   &timestamp, &value, &unit);

        REQUIRE(result == 3);
        REQUIRE(strcmp(timestamp, "2017-07-08T12:47:36") == 0);
        REQUIRE(strcmp(unit, "W") == 0);
        REQUIRE(value == 43.000000f);
    }


    SECTION("LWT message") {
        std::string res = lwtMessage(true);

        char *status;
        int result = json_scanf(res.c_str(), res.length(), "{ status: %Q }",
                                &status);

        REQUIRE(result == 1);
        REQUIRE(strcmp(status, "Online") == 0);
    }
}


