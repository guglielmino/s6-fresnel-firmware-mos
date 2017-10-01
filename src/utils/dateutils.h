#pragma once

#include <time.h>
#include <string>

std::string now() {
    time_t now;
    time(&now);
    char buf[sizeof "2011-10-08T07:07:09Z"];
    strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));
    // this will work too, if your compiler doesn't support %F or %T:
    //strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    return std::string(buf);
}
