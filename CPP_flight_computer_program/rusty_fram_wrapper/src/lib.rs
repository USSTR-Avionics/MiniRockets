#![no_main]
#![no_std]

use core::{ffi::{c_float}};
// use core::panic::PanicInfo;
// use core::time::Duration;
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
pub extern "C" fn return_delay_test()  -> u8
    {
    // let ten_millis = Duration::from_millis(500);

    return 0;
    }

#[no_mangle]
pub extern "C" fn int_wrap_temperature_for_writing(temp_reading: c_float) -> *const u8
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

    return temp_arr.as_ptr();
    }