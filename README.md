# USST Avionics Board

<center> <img src = https://i.imgur.com/jnRxNR3.png> </img> </center>

# Getting Started

1) Download and install <a href="https://www.arduino.cc/en/software">Arduino IDE 1.8.19</a>  
2) Download and install <a href="https://www.pjrc.com/teensy/td\_download.html">Teensyduino</a> 
3) Clone the git repo and open flight_computerprogram.ino 
4) Add the zip libraries in the libraries folder (Sketch-\>Include Library-\>Add .ZIP Library) 
5) Verify the code has no errors Upload to Teensy 4.1 microcontroller

# Using VSCode and PlatformIO (recommended)

1. Clone the codebase  
    `git clone https://github.com/USSTR-Avionics/Avionics-code.git`  

    [troubleshooting] if you get an error saying https protocol not supported, simply erase the https:// from the pasted url and rewrite it in, there is a 
    hiddend character that gets pasted in on Windows.  

2. Navigate to Extensions on VSCode and install "PlatoformIO IDE"  
    [note] restarting VSCode may be required and an internet connection is required!  

3. Select the "PIO Home" tab in VSCode > "Import Existing Project" > Navigate to the directory where the codebase was cloned >   
    open "CPP_flight_computer_code" > Scroll down to reveal the "Import Project" > Select board as "Teensy 4.1"  

4. At the bottom click on the checkbox to build the codebase  
