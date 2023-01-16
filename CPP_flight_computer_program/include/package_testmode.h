#ifndef PACKAGE_TESTMODE_H
#define PACKAGE_TESTMODE_H

#include <Arduino.h>

// NOTE:
// undefine ROCKET_DEBUG for production builds
#define ROCKET_DEBUGMODE 1
// #undef ROCKET_DEBUGMODE

#ifdef ROCKET_DEBUGMODE
    #define println(x) Serial.println(x)
    #define print(x) Serial.print(x)
#else
    #define println(x)
    #define print(x)
#endif  // ROCKET_DEBUGMODE

#include "statemachine_t.h"

statemachine_t::e_rocket_state enter_state(int state);
void test_mode_state();

#endif // PACKAGE_TESTMODE_H