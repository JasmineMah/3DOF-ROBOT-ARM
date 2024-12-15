# Introduction:
  This repository contains the source code for a 3-DOF arm project for CMPUT312. This project was started from scratch by Jasmine Mah and Eric Cheng. This code is designed to run on PlatformIO and Arduino IDE (though needs some additional setup). 

# Software:
**Testing:**
- Located in the `test/` folder

  - `test_basiclinearalgebra.cpp` - tests our implementations of math functions
  - `PWMTest.ino` - tests basic motor functionality (sweep in both directions)
  - `MotorEncoderAndLCDTest.ino` - tests encoder functionality with LCD display for debugging

**Source:**
- Located in the `src/` folder

  - `main.cpp` - records arm information and executes commands
  - `motors.cpp` - implementations for motor functionalities
  - `robot_arm.cpp` - implementations for robot arm specific functionalities
  - `webserver.cpp` - hosts the web server and displays arm data in a real-time fashion

**Libraries:**

- *LiquidCrystal* for displaying motor location values on the robot's on-board screen
- *Adafruit PWM Servo Driver*
- *Adafruit BusIO* (Dependent for servo driver library above)
- *WiFi & WebServer* (Built-into ESP32; simple WiFi connectivity and web server hosting capabilities)

# Hardware:
## Electronics: 
At the base of the arm, you will find a breadboard circuit containing the following:

- **Microcontroller:** *ESP-WROOM-32 Board* by Espressif. Known as ESP32dev on PlatformIO/ArduinoIDE.
- **Motor controller:** *PCA9685* PWM motor controller board
- **Motor encoder:** *AMT103-V* Shaft encoders
- **Power supply:** 5V 10A DC supply with breadboard adapter

## Mechanical: 
The arm is powered by 3 motors and is supported by two identical PDA 3D printed linkages

- **Base/Shoulder motor:** 20kg DS3218 digital servo motor
- **Elbow and wrist motor:** 20kg RDS3218 robot digital servo motors

# Resources:

## CAD Models:
  Below is a list of CAD models we borrowed from online. The base and linkages were created by us with the help of these models to be printed in the Elko Engineering garage

**Motors:**

- Base motor: https://grabcad.com/library/servo-motor-servo-20kg-1
- Arm motors and aluminum attachments: https://www.crealitycloud.com/model-detail/64dcf95c47473cf6002586b4

**Electronics:**

- Motor encoder: https://www.sameskydevices.com/product/resource/digikey3dmodel/amt10-series
- Motor controller:https://grabcad.com/library/servo-ds3218mg-1
- Breadboard: https://grabcad.com/library/breadboard-15
- 16x2 LCD screen: https://grabcad.com/library/16-x-2-lcd-display-module