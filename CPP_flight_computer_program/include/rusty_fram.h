#include <stdint.h>

extern "C" uint8_t get_u8_from_rust();
extern "C" int32_t pass_and_return_through_ffi(int32_t);
extern "C" uint8_t wrap_temperature_for_writing(int32_t);