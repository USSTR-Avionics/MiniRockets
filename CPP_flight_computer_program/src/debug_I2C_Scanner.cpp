#include "I2CScanner.h"

I2CScanner scanner;

/**
 * @brief Initialize the I2C scanner
 */
void init_I2C_scanner()
	{
	scanner = I2CScanner();
	scanner.Init();
	}

/**
 * @brief Scan the I2C bus and print the results
 */
void scan_and_print_I2C_devices()
	{
	scanner.Scan();
	}