#![no_main]
#![no_std]

use core::{convert::TryInto, ffi::c_int};
// use core::time::Duration;
// use core::panic::PanicInfo;
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
pub extern "C" fn wrap_temperature_for_writing(temp: c_int) -> u8 // take in a f32
    {
    if (temp <= 100) && (temp >= 0)
        {
        let unwrp_result: Result<u8, core::num::TryFromIntError> = temp.try_into(); 
        let wrapped_temp: u8 = match unwrp_result
            {
            Ok(t) => t + 101,
            _ => 0,
            };
        // wrapped_temp = wrapped_temp + 101;
        return wrapped_temp;
        }
    else if (temp <= 0) && (temp >= -100)
        {
        let temp_conv = temp + 101;
        let unwrp_result: Result<u8, core::num::TryFromIntError> = temp_conv.try_into(); 
        let wrapped_temp: u8 = match unwrp_result
            {
            Ok(t) => t,
            _ => 0,
            };
        // wrapped_temp = wrapped_temp + 101;
        return wrapped_temp;
        }

    return 0;
    }

/* 
// Sam's take on this fn
#[no_mangle]
pub extern "C" fn Sam_wrap_temperature_for_writing(temp: c_int) -> u8
{
    if (temp > 100) || (temp < -100)
    {
        0
    }
    else
    {
        (temp + 101) as u8
    }
    }
*/