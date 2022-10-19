# Sensor Chunk Ordering
**NOTE: The ordering matters as they are read and written in a different manner**

| Sr No | Data | Sensor | Bytes occupied | Precision | Original data type | Units |
|---|---|---|---|---|---|---|
| 1. | Temperature |    | 2 | 2 decimal points | float | celsius |
| 2. | Barometric pressure | MS5611 |    |     | float |
| 3.  Derieved Altitude | from program |
| 4.  Pressure | BMP280 |
| 5.  Accelerometer data | KX134 |
| | | | 2 | |  |




# Temperature layout

- Take up two bytes on the fram

| byte 1     | byte 2       |  
| whole part | decimal part |

- accepted precision from -100 to 100, and
  - this limited range is acceptable as the temperature sensor can only reliably read -10 to 85 C, so any reading beyond the range is unreliable
- two digits for decimal part from .00 to .99
 
0 => reserved as an error

## Writing temperature

INPUT:
    The program can input any i32

PROCESS:
    if i32 input out of range then write 0;
    if i32 input <= 100 and i32 input >= -100, add 101 then write num (min: 0, max: 201)
    else write 0;

OUTPUT:
    Range: 0-255 (limited by FRAM only accepting an u8)

## Reading temperature

- Read and subtract 101, and if difference isnt 0 then valid else invalid