# University of Saskatchewan Space Design Team: Avionics

[![PlatformIO CI master](https://github.com/USSTR-Avionics/Avionics_code/actions/workflows/building_test.yml/badge.svg?branch=master)](https://github.com/USSTR-Avionics/Avionics_code/actions/workflows/building_test.yml)
[![test-clang-format-lint](https://github.com/USSTR-Avionics/Avionics_code/actions/workflows/clang-format-check.yml/badge.svg)](https://github.com/USSTR-Avionics/Avionics_code/actions/workflows/clang-format-check.yml)
![open issues](https://img.shields.io/github/issues-raw/USSTR-Avionics/Avionics_code)
![](https://img.shields.io/github/issues-pr-raw/USSTR-Avionics/Avionics_code)
![](https://badgen.net/badge/teensy_kill_count/1/red)
---

# Rocket Autonomy System

Welcome to the Rocket Autonomy System project, developed by the University of Saskatchewan Space Design Team (USST). This software project aims to create a complete autonomous system for rockets.

## Project Overview

The primary goal of this project is to design and implement an autonomous system for rockets that exhibits three main capabilities:

1. **Command**: The system is designed to send/receive commands to/from a ground station in real-time. It should also be capable of executing received commands on the rocket promptly.

2. **Control**: The system will autonomously control the rocket, ensuring stable flight trajectories. This includes phases such as powered flight, ballistic descent, and controlled parachute deployment.

3. **Data Acquisition**: The system must collect data from various sensors within the rocket. Collected data should be transmitted to the ground station in real-time, enabling real-time monitoring and analysis of the rocket's performance during flight.

## Key Objectives

The Rocket Autonomy System project encompasses the following key objectives:

- **Parachute Deployment**: The system will implement logic for accurate deployment of a parachute at the rocket's apogee, ensuring a safe descent and recovery.

- **Data Collection and Storage**: The system will gather sensor data from onboard components, including accelerometers, barometric pressure sensors, thermocouples, and other relevant sensors. Collected data will be stored for analysis.

- **Ground Station Communication**: The system will establish a robust communication link between the rocket and a ground station. This link enables real-time exchange of commands and telemetry data, enhancing situational awareness and control.

## Mini Rocket Flight Computer Prototype

The project began with the development of the Mini Rocket Flight Computer prototype. This early prototype was crafted using commercial off-the-shelf breakout boards assembled onto PCBs. The prototype aimed to demonstrate the system's capabilities, including:

- Parachute deployment
- Data logging from sensors

### PCB Design

The PCB design was completed using Eagle, a commonly used PCB design tool.

### Version 1 (Engineering Model)

The initial version, known as the Engineering Model, served as a proof of concept. It successfully demonstrated the planned functionalities, laying the foundation for the Rocket Autonomy System project.

### Version 2 (Flight Model)

The second version, referred to as the Flight Model, brought significant enhancements to the Mini Rocket Flight Computer:

- Added additional breakout boards, including a thermocouple and a 32kByte FRAM for expanded data storage capacity.
- Improved power trace reliability for enhanced performance and reliability.
- Incorporated design modifications, including a gap in the middle of the system to meet aerostructure requirements.

#### Sensors

A variety of commercial off-the-shelf breakout sensors were utilized to develop and test the design rapidly. The design involved testing the following breakout boards:

- Teensy 4.1 Development Board (PJRC)
- KX134 Breakout (SparkFun)
- BNO055 IMU Breakout (Adafruit)
- BMI088 IMU Breakout (Mouser)
- MS5611 Breakout (Smart Prototyping)
- BMP280 Barometer (Adafruit)
- 32kByte FRAM Breakout (Adafruit)
- MAX31855 Thermocouple Breakout (Adafruit)
- RFM95W LoRa Radio Transceiver Breakout (Adafruit)
- DMC809 Servo Motors (RobotShop) (Not used in the final design)

#### Software

The software was written in C++ using PlatformIO in Visual Studio Code.

#### Lessons Learned

Through the development process, the team gained valuable insights:

- Teensy 4.1 is highly efficient for rapid code development.
- KX134 accelerometer is exceptionally capable, measuring up to 64g's of acceleration.
- BNO055's sensor fusion algorithms aren't suitable for rockets due to their dependency on downward acceleration being 9.81m/s^2.
- BMI088 worked well, but its non-standard header pins posed challenges for breadboard testing.
- MS5611 and BMP280 performed effectively.
- 32kBytes of FRAM storage proved barely sufficient for minimal rocket data, even with data compression.
- MAX31855 reliably operated, even with thermocouples from different suppliers.
- RFM95W LoRa Radios functioned well, though integrating libraries into PlatformIO presented some difficulties.
- Servo motors are unsuitable for parachute deployment systems.
- Modularization was vital, with the main program containing only essential components.

Thank you for your interest in the Rocket Autonomy System project. Together, we can push the boundaries of rocket technology and contribute to the advancement of autonomous rocket systems.

<p align="center"> <img src = https://i.imgur.com/jnRxNR3.png> </img> </p>
