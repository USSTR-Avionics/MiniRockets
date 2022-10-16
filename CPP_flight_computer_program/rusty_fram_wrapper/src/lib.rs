#![no_main]
#![no_std]

use panic_halt as _;

#[no_mangle]
pub extern "C" fn get_u8_from_rust()  -> u8
    {
    1
    }
