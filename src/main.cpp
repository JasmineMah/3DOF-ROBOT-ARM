#include <Arduino.h>

// // put function declarations here:
// int myFunction(int, int);

// void setup() {
//   // put your setup code here, to run once:
//   int result = myFunction(2, 3);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
// }

// // put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PWMServoDriver.h>

// Initialize PCA9685 at default I2C address 0x40
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver();

void setup() {
    Wire.begin();
    pca9685.begin();
    pca9685.setPWMFreq(50);  // Set frequency to 50Hz for servos
}

void loop() {
    int pulseLengthDS3218[3] = {400, 450, 500};  // Adjust PWM values for each motor

    // Loop through the three DS3218 motors connected to ports 0, 1, and 2
    for (int i = 0; i < 3; i++) {
        pca9685.setPWM(i, 0, pulseLengthDS3218[i]);  // Set the PWM signal for each motor
    }

    delay(1000);  // Adjust delay for movement timing
}
