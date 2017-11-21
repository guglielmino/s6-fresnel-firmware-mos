#pragma once

#include <time.h>
#include <string>

std::string now() {
    time_t now;
    time(&now);
    char buf[sizeof "2011-10-08T07:07:09Z"];
    strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));
    return std::string(buf);
}

time_t utc_now() {
    time_t now;
    time(&now);
    return now;
}

time_t utc_midnight() {
    return utc_now() / 86400 * 86400;
}
