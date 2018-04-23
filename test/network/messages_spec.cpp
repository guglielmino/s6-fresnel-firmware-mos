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
        std::vector<std::string> arr;

        /* TODO: In test there is a problem with the filled vector
        arr.push_back("POWERMETER");
        arr.push_back("RELAY1");
        arr.push_back("RELAY2");
        */

        std::string res = devInfoMessage("2018-04-07T13:16:45Z", "Sample", "1.0.0", "a group", "a name",
                                         "1.0.0.0", "2.0.0.0", arr);

        char *timestamp, *appName, *version, *group, *name, *wifiIp, *ethIp;
        json_scanf(res.c_str(), res.length(),
                   "{ timestamp: %Q, appName: %Q, version: %Q, group: %Q, name: %Q, wifi: {ip: %Q}, eth: {ip: %Q}, features: %M }",
                   &timestamp, &appName, &version, &group, &name, &wifiIp, &ethIp, scan_array);

        REQUIRE(res.length() > 0);
        REQUIRE(strcmp(timestamp, "2018-04-07T13:16:45Z") == 0);
        REQUIRE(strcmp(appName, "Sample") == 0);
        REQUIRE(strcmp(version, "1.0.0") == 0);
        REQUIRE(strcmp(group, "a group") == 0);
        REQUIRE(strcmp(name, "a name") == 0);
        REQUIRE(strcmp(wifiIp, "1.0.0.0") == 0);
        REQUIRE(strcmp(ethIp, "2.0.0.0") == 0);
    }

    SECTION("power feedback message") {
        std::string res = powerFeedbackMessage("2018-04-07T13:16:45Z", true, 1);

        char *status, *timestamp;
        int idx;
        json_scanf(res.c_str(), res.length(), "{ timestamp: %Q, status: %Q, relay_idx: %d }",
                   &timestamp, &status, &idx);

        REQUIRE(res.length() > 0);
        REQUIRE(strcmp(timestamp, "2018-04-07T13:16:45Z") == 0);
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
        std::string res = lwtMessage("2017-07-08T12:47:36", true);

        char *status, *timestamp;
        int result = json_scanf(res.c_str(), res.length(), "{ timestamp: %Q, status: %Q }",
                                &timestamp, &status);

        REQUIRE(result == 2);
        REQUIRE(strcmp(timestamp, "2017-07-08T12:47:36") == 0);
        REQUIRE(strcmp(status, "Online") == 0);
    }
}


