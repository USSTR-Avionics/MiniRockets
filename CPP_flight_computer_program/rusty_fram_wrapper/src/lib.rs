#![no_main]
#![no_std]

use core::ffi::{c_float};
use panic_halt as _;

/*
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
*/

#[no_mangle]
pub extern "C" fn get_u8_from_rust()  -> u8
    {
    1
    }

#[no_mangle]
pub extern "C" fn rust_return_delay_test()  -> u8
    {
    let limit: u128 = 100_000_000_000; // The time is probably 122348ms 
    let mut count: u128 = 0;

    while count < limit
        {
        count += 1;
        }

    return 0;
    }

#[repr(C)]
struct StructTemperature
    {
    first: u8,
    second: u8,
    }

impl StructTemperature
    {
    fn new(first: u8, second: u8) -> Self
        {
        StructTemperature { first, second }
        }
    }

#[no_mangle]
pub extern "C" fn wrap_temperature_for_writing(temp_reading: c_float) -> *const u8
    {
    let mut temp_whole: i32 = temp_reading as i32;
    if (temp_whole >= -100) && (temp_whole <= 100)
        {
        temp_whole += 101;
        }
    else
        {
        temp_whole = 0;
        }

    let mut temp_decimal: i32 = ((temp_reading * 100.0) as i32) % 100;
    if (temp_decimal < 0) && (temp_decimal > 99)
        {
        temp_decimal = 0;
        }

    let temp_arr: [u8; 2] = [temp_whole as u8, temp_decimal as u8]; 
    let temp_str: StructTemperature = StructTemperature::new(temp_whole as u8, temp_decimal as u8);

    return temp_arr.as_ptr();
    }

#[no_mangle]
pub extern "C" fn wrap_acceleration_for_writing(acc: c_float) -> *const u8
    {
    let ptr: [u8; 4] = acc.to_le_bytes(); // little endian encoding 
    return ptr.as_ptr();
    }