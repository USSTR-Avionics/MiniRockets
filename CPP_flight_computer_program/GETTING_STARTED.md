## Prerequisites

- git bash 

- VSCode 



## Cloning and Building the codebase on your machine

1. Clone the codebase
    `git clone https://github.com/USSTR-Avionics/Avionics-code.git`

    [troubleshooting] if you get an error saying https protocol not supported, simply erase the https:// from the pasted url and rewrite it in, there is a 
    hiddend character that gets pasted in on Windows.

2. Navigate to Extensions on VSCode and install "PlatoformIO IDE" 
    
    [note] restarting VSCode may be required and an internet connection is required!

3. Select the "PIO Home" tab in VSCode > "Import Existing Project" > Navigate to the directory where the codebase was cloned > 
    open "CPP_flight_computer_code" > Scroll down to reveal the "Import Project" > Select board as "Teensy 4.1"

4. At the bottom click on the checkbox to build the codebase


## Recommended VSCode Extensions

1. GitLens by GitKraken
    - so you know who to blame for the broken code

2. BetterComments by Aaron Bond

3. Prettier