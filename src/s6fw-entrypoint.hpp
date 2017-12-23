#include <string>
#include <time.h>

#include "mgos.h"
#include "mgos_timers.h"
#include "mgos_config.h"

#include "consts.h"
#include "factories/sensorsFactories.h"
#include "globals.h"

#include "setup/cron_sys.hpp"
#include "setup/sensors_sys.hpp"
#include "setup/mqtt_sys.hpp"
#include "setup/devices_sys.hpp"


enum mgos_app_init_result mgos_app_init(void) {
    cs_log_set_level(LL_DEBUG);
    LOG(LL_DEBUG, ("Device ID %s", settings.deviceId()));

    mqtt_sys_init();

    sensors_sys_init();

    // Start Kw/h accumulation on MCP39F511
    ISensorCommand *startDailyKwhCounter = getStartDailyKkhCommand();
    startDailyKwhCounter->exec();

    devices_sys_init();

    cron_sys_init();

    return MGOS_APP_INIT_SUCCESS;
}
