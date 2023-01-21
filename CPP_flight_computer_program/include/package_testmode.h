#ifndef PACKAGE_TESTMODE_H
#define PACKAGE_TESTMODE_H

#include <Arduino.h>

// NOTE:
// undefine ROCKET_DEBUG for production builds
#define ROCKET_DEBUGMODE 1
#undef ROCKET_DEBUGMODE


// DEBUG MODE dependent code and definitions go here!
#ifdef ROCKET_DEBUGMODE
    unsigned long debug_time = 0UL;
    #warning "DEBUG MODE ENABLED"
    #define println(x) Serial.println(x)
    #define print(x) Serial.print(x)
#else
    #define println(x)
    #define print(x)
    #error "ARE YOU SURE YOU WANT TO BUILD IN PRODUCTION MODE?"
    #warning "DELETE THE ERRONEOUS LINE TO BUILD IN PRODUCTION MODE"
#endif  // ROCKET_DEBUGMODE

#include "statemachine_t.h"

statemachine_t::e_rocket_state enter_state(int state);
void test_mode_state();

#endif // PACKAGE_TESTMODE_H
