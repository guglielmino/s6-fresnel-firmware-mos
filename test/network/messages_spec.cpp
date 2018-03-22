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

    SECTION("create a device info message") {
        std::vector<const char *> arr = { "POWERMETER", "RELAY1", "RELAY2"};

        std::string res = devInfoMessage("Sample", "1.0.0", "a group", "a name", arr);
        std::cout << res << "\n\n";

        char *appName, *version, *group, *name;
        json_scanf(res.c_str(), res.length(), "{ appName: %Q, version: %Q, group: %Q, name: %Q, features: %M }",
                   &appName, &version, &group, &name, scan_array);

        REQUIRE(res.length() > 0);
        REQUIRE(strcmp(appName, "Sample") == 0);
        REQUIRE(strcmp(version, "1.0.0") == 0);
        REQUIRE(strcmp(group, "a group") == 0);
        REQUIRE(strcmp(name, "a name") == 0);
    }

}


