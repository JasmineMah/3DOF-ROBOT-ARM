# Introduction:
  This repository contains the source code for a 3-DOF arm project for CMPUT312. This project was started from scratch by Jasmine Mah and Eric Cheng. This code is designed to run in and was tested on Arduino IDE. 

# Software:
**Testing:**
- **Motor sweeping:** *sweep.cpp* Moves motors back and forth in a loop. Arm rotates back and forth and bends elbow
- **LCD screen:** *helloLCD.cpp* Prints 'hello world' to the LCD screen on board
- **Server:** *servertest.cpp* Sends values to the esp32 board and displays it on the LCD screen

**Source:**
- **Webserver:** *server.cpp* ESP32 host runs webserver and takes values sent via http by client (user on laptop)
- **Forward Kinematics:** *fwdkin.cpp*, can run independently of webserver (hardcode values)
- **Inverse Kinematics:** *invkin.cpp*, can run independently of webserver (hardcode values)
- *main.cpp* void setup and loop, takes data from webserver and calls invkin. Allows users to log onto the robot by typing in the IP address and send commands

**Libraries:**
- *LCD screen*
- *Adafruit PWM Drivers*
- *I2C (Wire)*

# Hardware:
## Electronics: At the base of the arm, you will find a breadboard circuit containing the following:
- **Microcontroller:** *ESP-WROOM-32 Board* by Espressif. Known as ESP32dev on PlatformIO/ArduinoIDE.
- **Motor controller:** *PCA9685* PWM motor controller board
- **Motor encoder:** *AMT103-V* Shaft encoders
- **Power supply:** 5V 10A DC supply with breadboard adapter

## Mechanical: The arm is powered by 3 motors and is supported by two identical PDA 3D printed linkages
- **Base/Shoulder motor:** 20kg DS3218 digital servo motor
- **Elbow and wrist motor:** 20kg RDS3218 robot digital servo motors

# Resources:
## CAD Models:
  Below is a list of CAD models we borrowed from online. The base and linkages were created by us with the help of these models to be printed in the Elko Engineering garage
**Motors:**
- Base motor:
- Arm motors and aluminum attachments:
**Electronics:**
- Motor encoder: https://www.sameskydevices.com/product/resource/digikey3dmodel/amt10-series
- Motor controller:https://grabcad.com/library/servo-ds3218mg-1
- Breadboard:
- 16x2 LCD screen:

## Research:

