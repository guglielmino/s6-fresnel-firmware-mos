
#ifndef S6TARGET
#pragma message "S6 MAIN FIRMWARE"
#include "s6fw-entrypoint.hpp"
#elif S6TARGET == calibration
#pragma message "S6 CALIBRATION FIRMWARE"
#include "s6calib-entrypoint.hpp"
#endif

