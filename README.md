# USST Avionics

[![PlatformIO CI master](https://github.com/USSTR-Avionics/Avionics_code/actions/workflows/building_test.yml/badge.svg?branch=master)](https://github.com/USSTR-Avionics/Avionics_code/actions/workflows/building_test.yml)
![open issues](https://img.shields.io/github/issues-raw/USSTR-Avionics/Avionics_code)
![open PRs](https://img.shields.io/github/issues-pr/USSTR-Avionics/Avionics_code/open)


<center> <img src = https://i.imgur.com/jnRxNR3.png> </img> </center>


# Setup
Using VSCode and PlatformIO (recommended)


## Things you need to get started
- [ ] VSCode  
- [ ] platformIO extension  
- [ ] install git bash for your platform  
- [ ] install [Rust](https://www.rust-lang.org/tools/install) for your platform  


## Next Steps

1. Fork the codebase  
    `https://github.com/USSTR-Avionics/Avionics-code.git`  
    a. Visit the link and fork the code base to your own profle by simply clicking fork  
    b. Clone your own fork to your local machine  
    c. Add the upstream repo to get latest changes  
    d. Run this command which enables you to get the latest changes from master  
    `git remote add upstream https://github.com/USSTR-Avionics/Avionics-code.git`  

    [troubleshooting] if you get an error saying https protocol not supported, simply erase the https:// from the pasted url and rewrite it in, there is a 
    hiddend character that gets pasted in on Windows.  

2. Navigate to Extensions on VSCode and install "PlatoformIO IDE"  
    [note] restarting VSCode may be required and an internet connection is required!  

3. Select the "PIO Home" tab in VSCode > "Import Existing Project" > Navigate to the directory where the codebase was cloned >   
    open "CPP_flight_computer_code" > Scroll down to reveal the "Import Project" > Select board as "Teensy 4.1"  


>**NOTE : You may use any toolchain you like as long as it maintains compatibility and interoperability with "master", using different tools from the team also means that the team can't help you debug or troubleshoot as quickly!**

### Optional steps (not needed for now)

Navigate to the root of rusty_fram_wrapper and run the terminal command  
    `make`  

At the bottom click on the checkbox to build the codebase (this also links the rust lib)

run `pio run -t compiledb` if you need the latest compilation_commands.json file for clangd DAP interface.


## BUILDING

### Using the terminal
`pio build` while in the directory where platform.ini is located

### Using VSCode and PlatoformIO
Click on the :heavy_check_mark: on the bottom bar in VSCode


# Contributing

1. Make sure you have all the remote changes
    `git pull`
    `git fetch --all`
    `git pull upstream`
    `git fetch upstream --all`

2. Squash commits into a single commit if it makes sense for your changes

3. Open up a Pull Request with an appropriate title and explanantion of code changes


# Documentation

Documentation is not expected for code, as long as variables, methods, and files are resonably named and follow the naming standards laid out in READMEs
This is because we believe code is the best documentation as it never gets outdated, because if it gets outdated it doesn't compile anymore. Hence, will always remain up to date.
However, provide documentation wherever relevant

Javadoc style of comments are strongly recommended, but only after that function has been stabilised. Don't write documentation while developing.

```
/**
 * @brief      writes an u8 to the given FRAM address
 * @param      what    The u8 to be written
 * @param      where   The address where it is to be written
 * @return     int     0 on success; 1 on failure
*/

int write_to_fram(uint8_t what, uint8_t where)
    {
    // a black box
    }

```

# Coding Style Guidelines


## Naming things

- ALL names follow the snake_case convention this includes everything from functions to files

### Variables

- every variable must have a default value to prevent undefined behaviour!

- constants must be upper case

```
    #define A_CONST 100
    int BCONST = 12.34f
```

### Header files

These are broad guidelines and **NOT** rules, break them if it doesn't make sense

We classify headers into 4 types

1. Package
    A set of functions that serve a common function, eg: a statistics package
    prefixed with a "package_"

2. Debug
    Functions associated with easy debugging, eg: I2C Scanner, these are only
    activated or in use when a directive or variable is enabled, prefixed with
    a "debug_"

3. Sensor
    Functions that serve as a wrapper/API for sensors, sensors include
    peripherals like memory and sensors like acceleration. tldr; anything that
    has a hardware component that the flight computer interacts with is a 
    sensor, prefixed with a "sensor_"

4. Organisational/Misc
    Some headers are just for storing default variables or macros and fall
    under this categories, need not be prefixed.

**NOTE:** Other naming scheme maybe applied if it makes sense, for example, the 
    header file containing the enum definition for the statemachine is called
    statemachine_t.h as the _t suffix denotes type by convention.

### Source files

They should be named similar to their respective header files that expose the API function calls


## Indentation

- we will use the Whitesmiths style of indents, [reasoning](http://www.activeclickweb.com/whitesmiths/index.html)  

- in line tab indents, that means you indent after a command block like in Python and then continue in line  

```
    if (condition == true)
        {
        foobar();
        }
```

- use tabs to indent, 1 tab = 4 spaces  


## Print statements

- all print statements are macros that are defined or undefined based on what mode the project is compiling into

- every debug print statement should have a tag indicating what it is in reference to   
    Example  
        [ROCKET STATE] => indicates the statement is for debugging rocket state  
        [KX134] => indicates debugging for the respective sensor  


## if-else statements

- make them explicit

```
    if (0){}            // bad
    if (true){}         // bad
    if (state){}        // bad
    if (state == false) // good
```

- try to make them into switch case statements wherever convinient
