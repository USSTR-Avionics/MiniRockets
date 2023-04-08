#ifndef DEBUG_I2C_SCANNER_H
#define DEBUG_I2C_SCANNER_H

#define KX134_I2C_ADDRESS  0x1F
#define BMP280_I2C_ADDRESS 0x77
#define FRAM_I2C_ADDRESS   0x50
#define FRAM_I2C_ADDRESS_2 0x7C

void init_I2C_scanner();
void scan_and_print_I2C_devices();

#endif // DEBUG_I2C_SCANNER_H
