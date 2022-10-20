#![no_main]
#![no_std]

use panic_halt as _;

#[no_mangle]
pub extern "C" fn get_u8_from_rust()  -> u8
    {
    1
    }

#[no_mangle]
pub extern "C" fn wrap_temperature_for_writing(temp: i32) -> u8 // take in a f32
    {
    let mut wrapped_temp: u8 = 0;

    if (temp >= -100) && (temp <= 100)
        {
        wrapped_temp = temp.try_into().unwrap(); 
        wrapped_temp = wrapped_temp + 101;
        }

    return wrapped_temp;
    }