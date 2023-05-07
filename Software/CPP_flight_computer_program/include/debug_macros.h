#ifndef DEBUG_MACROS_H
#define DEBUG_MACROS_H

#include "package_testmode.h"
#include <Arduino.h>

#define WAIT_FOR_AEROSTRUCTURE_TO_DO_SHIT 600000

#ifdef ROCKET_DEBUGMODE
	#define println(x)     Serial.println(x)
	#define print(x)       Serial.print(x)

	#define DEBUG_INTERVAL 10

#else
	#define println(x)
	#define print(x)
#endif // ROCKET_DEBUGMODE

#endif // DEBUG_MACROS_H
