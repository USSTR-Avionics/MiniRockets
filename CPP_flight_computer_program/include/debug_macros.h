#ifndef DEBUG_MACROS_H
#define DEBUG_MACROS_H

#include <Arduino.h>
#include "package_testmode.h"

#ifdef ROCKET_DEBUGMODE
    #define println(x) Serial.println(x)
    #define print(x) Serial.print(x)
#else
    #define println(x)
    #define print(x)
#endif  // ROCKET_DEBUGMODE

#endif // DEBUG_MACROS_H