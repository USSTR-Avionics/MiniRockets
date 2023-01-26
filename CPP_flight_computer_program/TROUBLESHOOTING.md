Problem:
    `pio` command not found

Solution:
    Delete all current open terminals
    Command Pallete; "platformIO new terminal"

Problem:
    too many warnings from libraries?!

Solution:
    Most of them are from the .platformio toolchain, they can be supressed by manually adding the 
    `#pragma GCC diagnostic warning "-Wunused-variable"` replace the warning with the one you're
    seeing.
