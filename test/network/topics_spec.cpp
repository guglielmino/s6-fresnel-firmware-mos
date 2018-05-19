#include <cstring>
#include "../catch.hpp"
#include "../../src/network/topics.h"

TEST_CASE("topics must be created in the right format", "[topics]") {

    SECTION("create a device topic given the topic pattern") {
        char topic[100];
        makeDeviceTopic(topic, 100, PUB_SENS_INFO_TOPIC, "MYGW", "room1", "123");

        REQUIRE(strcmp(topic, "MYGW/room1/events/123/info") == 0);
    }

    SECTION("create a group topic given the topic pattern") {
        char topic[100];
        makeRoomTopic(topic, 100, SUB_SWITCH_ROOM, "MYGW", "room1");

        REQUIRE(strcmp(topic, "MYGW/room1/commands/power") == 0);
    }


}