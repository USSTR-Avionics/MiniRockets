#ifndef PACKAGE_TESTMODE_H
#define PACKAGE_TESTMODE_H

#include <Arduino.h>

// NOTE:
// undefine ROCKET_DEBUG for production builds
#define ROCKET_DEBUGMODE 1
// #undef ROCKET_DEBUGMODE


// DEBUG MODE dependent code and definitions go here!
#ifdef ROCKET_DEBUGMODE
	#warning "DEBUG MODE ENABLED"
#else
	#error "ARE YOU SURE YOU WANT TO BUILD IN PRODUCTION MODE?"
	#warning "DELETE THE ERRONEOUS LINE TO BUILD IN PRODUCTION MODE"
#endif // ROCKET_DEBUGMODE

#include "statemachine_t.h"

#endif // PACKAGE_TESTMODE_H