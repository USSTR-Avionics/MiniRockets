#ifndef PACKAGE_WATCHDOG_H
#define PACKAGE_WATCHDOG_H

#include "Watchdog_t4.h"

WDT_T4<WDT2> wdt;
WDT_timings_t config;

#define WATCHDOG_TRIGGER 3 // seconds
#define WATCHDOG_TIMEOUT 6 // seconds

void watchdog_callback();

#endif // PACKAGE_WATCHDOG_H