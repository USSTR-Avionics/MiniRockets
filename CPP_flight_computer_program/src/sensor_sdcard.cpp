#include "SD.h"

#define SD_DATA_FILE_NAME "datalog.txt"
const int chipSelect = 10;
File myFile;

int init_SD()
    {
    if (!SD.begin(chipSelect)) 
        {
        return EXIT_FAILURE;
        }

    bool result_rm = SD.remove(SD_DATA_FILE_NAME);
    bool result_mkdir = SD.mkdir(SD_DATA_FILE_NAME);

    if (!(result_mkdir && result_rm))
        {
        return EXIT_FAILURE;
        }

    return EXIT_SUCCESS;
    }

int write_to_sd_card(const char* str)
    {
    File dataFile = SD.open(SD_DATA_FILE_NAME, FILE_WRITE);

    if (dataFile == true) 
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