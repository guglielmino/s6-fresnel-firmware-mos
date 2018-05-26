#include "mgos.h"
#include "mgos_timers.h"
#include "mgos_config.h"

#include <string>
#include <time.h>

#include "consts.h"
#include "factories/uartFactories.h"
#include "globals.h"

#include "devfunctions.h"

#include "setup/cron_sys.hpp"
#include "setup/mqtt_sys.hpp"
#include "setup/devices_sys.hpp"

void event_sntp_timechange_cb(int ev, void *evd, void *arg)
{
    (void) ev;
    (void)arg;

    publishInfoMessage();
}


enum mgos_app_init_result mgos_app_init(void) {
    cs_log_set_level(LL_DEBUG);
    LOG(LL_DEBUG, ("Device ID %s", settings.deviceId()));

    // Firmware autocommit, dangerous for rollback handling
    if(!mgos_upd_is_committed()) {
        LOG(LL_DEBUG, ("UNCOMMITTED Firmware"));
        mgos_upd_commit();
    } else {
        LOG(LL_DEBUG, ("COMMITTED Firmware"));
    }


    mqtt_sys_init();

    devices_sys_init();

    dev_functions_init();

    // Start Kw/h accumulation on MCP39F511
    startKWhCounter();

    cron_sys_init();

    mgos_event_add_handler(MGOS_EVENT_TIME_CHANGED, event_sntp_timechange_cb, NULL);
    return MGOS_APP_INIT_SUCCESS;
}
