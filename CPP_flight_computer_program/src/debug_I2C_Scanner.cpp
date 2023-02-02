#include "I2CScanner.h"

I2CScanner scanner;

void scan_and_print_I2C_devices()
	{
	scanner.Scan();
	}