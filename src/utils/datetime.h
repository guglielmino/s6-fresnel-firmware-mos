#pragma once

#include <sstream>
#include <iomanip>
#include <string>

std::string now() {
    time_t now = time(0);
    std::stringstream buffer;
    buffer << std::put_time(gmtime(&now), "%Y-%m-%dT%H:%M:%S.000Z");
    return buffer.str();
}