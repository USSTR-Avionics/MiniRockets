#include "I2CScanner.h"

I2CScanner scanner;

void init_I2C_scanner()
	{
	scanner = I2CScanner();
	scanner.Init();
	}

void scan_and_print_I2C_devices()
	{
	scanner.Scan();
	}