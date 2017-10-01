//
// Created by Fabrizio Guglielmino on 01/10/17.
//

#pragma once

#include "mgos_gpio.h"
#include "mgos_timers.h"

/*********************************************************************************************\
 * HLW8012 - Energy
 *
 * Based on Source: Shenzhen Heli Technology Co., Ltd
 * Based on Sonoff-MQTT-OTA-Arduino sources
 * ref : https://raw.githubusercontent.com/arendst/Sonoff-MQTT-OTA-Arduino/master/sonoff/xsns_hlw8012.ino
\*********************************************************************************************/

#define HLW_PREF_PULSE         12530        // was 4975us = 201Hz = 1000W
#define HLW_UREF_PULSE         1950         // was 1666us = 600Hz = 220V
#define HLW_IREF_PULSE         3500         // was 1666us = 600Hz = 4.545A

#define HLW_PREF            10000    // 1000.0W
#define HLW_UREF             2200    // 220.0V
#define HLW_IREF             4545    // 4.545A

#define WS2812_PIN           14           // GPIO 14 option
#define WS2812_LEDS          30           // [Pixels] Number of LEDs

#define HLW_SEL              5            // GPIO 05 = HLW8012 Sel output (Sonoff Pow)
#define HLW_CF1              13           // GPIO 13 = HLW8012 CF1 voltage / current (Sonoff Pow)
#define HLW_CF               14           // GPIO 14 = HLW8012 CF power (Sonoff Pow)

/// -------------------------
/////  REFACTORING MOCKUP


int rtc_loctime() {
    return 0;
}

int rtc_midnight() {
    return 0;
}

struct SYSCFG {
    unsigned long hlw_kWhtoday;
    unsigned long hlw_kWhyesterday;
    uint16_t hlw_kWhdoy;
    unsigned long hlw_pcal;
    unsigned long hlw_ucal;
    unsigned long hlw_ical;
    uint16_t tele_period;

} sysCfg;

uint8_t power;                        // Current copy of sysCfg.power

struct TIME_T {
    uint8_t Second;
    uint8_t Minute;
    uint8_t Hour;
    uint8_t Wday;      // day of week, sunday is day 1
    uint8_t Day;
    uint8_t Month;
    char MonthName[4];
    uint16_t DayOfYear;
    uint16_t Year;
    unsigned long Valid;
} rtcTime;


/// -------------------------

/**
 * Emulate Arduino micros() function
 * @return
 */
unsigned long micros() {
    return (unsigned long) mgos_uptime * 1000000;
}

int8_t hlw_SELflag, hlw_cf_timer, hlw_cf1_timer, hlw_fifth_second, hlw_startup;
unsigned long hlw_cf_plen, hlw_cf_last;
unsigned long hlw_cf1_plen, hlw_cf1_last, hlw_cf1_ptot, hlw_cf1_pcnt, hlw_cf1u_plen, hlw_cf1i_plen;
unsigned long hlw_Ecntr, hlw_EDcntr, hlw_kWhtoday;
uint32_t hlw_lasttime;

unsigned long hlw_cf1u_pcntmax, hlw_cf1i_pcntmax;

#if WS2812_PIN != 3
void hlw_cf_interrupt(int pin, void *arg) ICACHE_RAM_ATTR;
void hlw_cf1_interrupt(int pin, void *arg) ICACHE_RAM_ATTR;
#endif

void hlw_cf_interrupt(int pin, void *arg)  // Service Power
{
    (void) pin;
    (void) arg;
    hlw_cf_plen = micros() - hlw_cf_last;
    hlw_cf_last = micros();
    if (hlw_cf_plen > 4000000) hlw_cf_plen = 0;  // Just powered on
    hlw_cf_timer = 15;  // Support down to 4W which takes about 3 seconds
    hlw_EDcntr++;
    hlw_Ecntr++;
}

void hlw_cf1_interrupt(int pin, void *arg)  // Service Voltage and Current
{
    (void) pin;
    (void) arg;
    hlw_cf1_plen = micros() - hlw_cf1_last;
    hlw_cf1_last = micros();
    if ((hlw_cf1_timer > 2) && (hlw_cf1_timer < 8)) {  // Allow for 300 mSec set-up time and measure for up to 1 second
        hlw_cf1_ptot += hlw_cf1_plen;
        hlw_cf1_pcnt++;
        if (hlw_cf1_pcnt == 10)
            hlw_cf1_timer = 8;  // We need up to ten samples within 1 second (low current could take up to 0.3 second)
    }
}

void hlw_200mS(void *param) {
    (void) param;
    unsigned long hlw_len, hlw_temp;

    hlw_fifth_second++;
    if (hlw_fifth_second == 5) {
        hlw_fifth_second = 0;

        if (hlw_EDcntr) {
            hlw_len = 1000000 / hlw_EDcntr;
            hlw_EDcntr = 0;
            hlw_temp = (HLW_PREF * sysCfg.hlw_pcal) / hlw_len;
            hlw_kWhtoday += (hlw_temp * 100) / 36;
        }
        if (rtc_loctime() == rtc_midnight()) {
            sysCfg.hlw_kWhyesterday = hlw_kWhtoday;
            hlw_kWhtoday = 0;
        }
        if (hlw_startup && rtcTime.Valid && (rtcTime.DayOfYear == sysCfg.hlw_kWhdoy)) {
            hlw_kWhtoday = sysCfg.hlw_kWhtoday;
            hlw_startup = 0;
        }
    }

    if (hlw_cf_timer) {
        hlw_cf_timer--;
        if (!hlw_cf_timer) hlw_cf_plen = 0;  // No load for over three seconds
    }

    hlw_cf1_timer++;
    if (hlw_cf1_timer >= 8) {
        hlw_cf1_timer = 0;
        hlw_SELflag = (hlw_SELflag) ? 0 : 1;
        mgos_gpio_write(HLW_SEL, (hlw_SELflag == 1));

        if (hlw_cf1_pcnt) {
            hlw_cf1_plen = hlw_cf1_ptot / hlw_cf1_pcnt;
        } else {
            hlw_cf1_plen = 0;
        }
        if (hlw_SELflag) {
            hlw_cf1u_plen = hlw_cf1_plen;
            hlw_cf1u_pcntmax = hlw_cf1_pcnt;
        } else {
            hlw_cf1i_plen = hlw_cf1_plen;
            hlw_cf1i_pcntmax = hlw_cf1_pcnt;
        }
        hlw_cf1_ptot = 0;
        hlw_cf1_pcnt = 0;
    }
}

void hlw_savestate() {
    sysCfg.hlw_kWhdoy = (rtcTime.Valid) ? rtcTime.DayOfYear : 0;
    sysCfg.hlw_kWhtoday = hlw_kWhtoday;
}

uint8_t hlw_readEnergy(uint8_t option, float &ed, uint16_t &e, uint16_t &w, uint16_t &u, float &i, float &c) {
    unsigned long hlw_len, hlw_temp, hlw_w = 0, hlw_u = 0, hlw_i = 0;
    int hlw_period, hlw_interval;

//char log[LOGSZ];
//snprintf_P(log, sizeof(log), PSTR("HLW: CF %d, CF1U %d (%d), CF1I %d (%d)"), hlw_cf_plen, hlw_cf1u_plen, hlw_cf1u_pcntmax, hlw_cf1i_plen, hlw_cf1i_pcntmax);
//addLog(LOG_LEVEL_DEBUG, log);

    if (hlw_kWhtoday) {
        ed = (float) hlw_kWhtoday / 100000000;
    } else {
        ed = 0;
    }

    if (option) {
        if (!hlw_lasttime) {
            hlw_period = sysCfg.tele_period;
        } else {
            hlw_period = rtc_loctime() - hlw_lasttime;
        }
        hlw_lasttime = rtc_loctime();
        hlw_interval = 3600 / hlw_period;
        if (hlw_Ecntr) {
            hlw_len = hlw_period * 1000000 / hlw_Ecntr;
            hlw_Ecntr = 0;
            hlw_temp = ((HLW_PREF * sysCfg.hlw_pcal) / hlw_len) / hlw_interval;
            e = hlw_temp / 10;
        } else {
            e = 0;
        }
    }

    if (hlw_cf_plen) {
        hlw_w = (HLW_PREF * sysCfg.hlw_pcal) / hlw_cf_plen;
        w = hlw_w / 10;
    } else {
        w = 0;
    }
    if (hlw_cf1u_plen && (w || (power & 1))) {
        hlw_u = (HLW_UREF * sysCfg.hlw_ucal) / hlw_cf1u_plen;
        u = hlw_u / 10;
    } else {
        u = 0;
    }
    if (hlw_cf1i_plen && w) {
        hlw_i = (HLW_IREF * sysCfg.hlw_ical) / hlw_cf1i_plen;
        i = (float) hlw_i / 1000;
    } else {
        i = 0;
    }
    if (hlw_i && hlw_u && hlw_w && w) {
        hlw_temp = (hlw_w * 100) / ((hlw_u * hlw_i) / 1000);
        if (hlw_temp > 100) {
            hlw_temp = 100;
        }
        c = (float) hlw_temp / 100;
    } else {
        c = 0;
    }

    return true;
}

void hlw_init() {
    mgos_uptime_init();
    mgos_gpio_init();

    if (!sysCfg.hlw_pcal || (sysCfg.hlw_pcal == 4975)) {
        sysCfg.hlw_pcal = HLW_PREF_PULSE;
        sysCfg.hlw_ucal = HLW_UREF_PULSE;
        sysCfg.hlw_ical = HLW_IREF_PULSE;
    }

    hlw_cf_plen = 0;
    hlw_cf_last = 0;
    hlw_cf1_plen = 0;
    hlw_cf1_last = 0;
    hlw_cf1u_plen = 0;
    hlw_cf1i_plen = 0;
    hlw_cf1u_pcntmax = 0;
    hlw_cf1i_pcntmax = 0;

    hlw_Ecntr = 0;
    hlw_EDcntr = 0;
    hlw_kWhtoday = 0;

    hlw_SELflag = 0;  // Voltage;

    mgos_gpio_set_mode(HLW_SEL, MGOS_GPIO_MODE_OUTPUT);
    mgos_gpio_write(HLW_SEL, (hlw_SELflag == 1));
    mgos_gpio_set_mode(HLW_CF1, MGOS_GPIO_MODE_INPUT);
    mgos_gpio_set_pull(HLW_CF1, MGOS_GPIO_PULL_UP);
    mgos_gpio_set_int_handler_isr(HLW_CF1, MGOS_GPIO_INT_EDGE_NEG, hlw_cf1_interrupt, NULL);
    mgos_gpio_set_mode(HLW_CF, MGOS_GPIO_MODE_INPUT);
    mgos_gpio_set_pull(HLW_CF, MGOS_GPIO_PULL_UP);
    mgos_gpio_set_int_handler_isr(HLW_CF, MGOS_GPIO_INT_EDGE_NEG, hlw_cf_interrupt, NULL);

    hlw_startup = 1;
    hlw_lasttime = 0;
    hlw_fifth_second = 0;
    hlw_cf_timer = 0;
    hlw_cf1_timer = 0;
    mgos_set_timer(200, 1, hlw_200mS, NULL);
}


