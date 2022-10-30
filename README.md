# USST Avionics Board

<center> <img src = https://i.imgur.com/jnRxNR3.png> </img> </center>

# Getting Started (:warning: deprecated)

1) Download and install <a href="https://www.arduino.cc/en/software">Arduino IDE 1.8.19</a>  
2) Download and install <a href="https://www.pjrc.com/teensy/td\_download.html">Teensyduino</a> 
3) Clone the git repo and open flight_computerprogram.ino 
4) Add the zip libraries in the libraries folder (Sketch-\>Include Library-\>Add .ZIP Library) 
5) Verify the code has no errors Upload to Teensy 4.1 microcontroller

# Using VSCode and PlatformIO (recommended)

## Things you need to get started
- [ ] VSCode  
- [ ] platformIO extension  
- [ ] install git bash for your platform  
- [ ] install rustc and cargo for your platform  


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

4. Navigate to the root of rusty_fram_wrapper and run the terminal command  
    `make`  

5. At the bottom click on the checkbox to build the codebase (this also links the rust lib)


**NOTE : You may use any toolchain you like as long as it maintains compatibility and interoperability with "master", using different tools from the team also means that the team can't help you debug or troubleshoot as quickly**
