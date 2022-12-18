## Get the library

Download the source code usually a .zip file, add the zip file to the `archive_lib` folder

Unizip the .zip file and add it to the `lib` folder, ensure it conforms it to the following structure

SensorABC
    |
    - src
        |
        - SensorABC.cpp
        - SensorABC.h
        |
    - examples
        |
        - readFromSensor.ino
    - docs
        |
        - documentation.md

**The only important thing here is to name the folder SensorABC the same as the header file**

Create a new header file in the headers folder and add a new header, this header is included in the `main.cpp`
folder, this header `#include`s the `SensorABC.h` this allows us to add a layer of abstraction to abstract 
away library details from the devs on the main code base.
