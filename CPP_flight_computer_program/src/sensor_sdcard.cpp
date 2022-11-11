#include "SD.h"

#define BUILTIN_SDCARD 254;

const int chipSelect = BUILTIN_SDCARD;
File myFile;

int init_SD()
    {
    if (!SD.begin(chipSelect)) 
        {
        return EXIT_FAILURE;
        }

    bool result_rm = SD.remove("datalog.txt");
    bool result_mkdir = SD.mkdir("datalog.txt");

    if (!(result_mkdir && result_rm))
        {
        return EXIT_FAILURE;
        }

    return EXIT_SUCCESS;
    }

int write_to_sd_card(const char* str)
    {
    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    if (dataFile) 
        {
        dataFile.println(str);
        dataFile.close();
        return EXIT_SUCCESS;
        }
    else 
        {
        return EXIT_FAILURE;
        }
    }