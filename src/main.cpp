#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Adafruit_PWMServoDriver.h>

// Initialize the LCD with the pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(14, 12, 27, 26, 25, 33);

// Servo pulse range (adjust if necessary for your servos)
#define SERVOMIN 10  // Minimum pulse length
#define SERVOMAX 150  // Maximum pulse length
Adafruit_PWMServoDriver pwm;

void setup() {
    // Setup for LCD
    lcd.begin(16, 2); // Initialize 16x2 LCD
    lcd.print("Hello, LCD!"); // Print test message
    delay(2000); // Pause for 2 seconds
    lcd.clear();

    // Setup for Serial
    Serial.begin(115200); // Start Serial at 115200 baud
    delay(1000);
    Serial.println("Hello, Serial Monitor!");

    // Setup for PCA9685
    pwm = Adafruit_PWMServoDriver();
    Wire.begin();
    pwm.begin();
    pwm.setPWMFreq(50); // Set frequency to 50Hz for servo control
    delay(10);

    Serial.println("PCA9685 Initialized. Setting initial positions...");

    // Set initial servo position
    pwm.setPWM(0, 0, SERVOMIN);
    delay(1000);

    lcd.print("Setup Complete!");
    Serial.println("Setup complete. Starting loop...");
}

void loop() {
    // Move servos forward
    Serial.println("Sweeping servos forward...");
    lcd.clear();
    lcd.print("Moving Forward...");
    for (int pulseWidth = SERVOMIN; pulseWidth <= SERVOMAX; pulseWidth++) {
        pwm.setPWM(0, 0, pulseWidth); // Servo 1 (channel 0)
        delay(10);
    }

    // Move servos backward
    Serial.println("Sweeping servos backward...");
    lcd.clear();
    lcd.print("Moving Backward...");
    for (int pulseWidth = SERVOMAX; pulseWidth >= SERVOMIN; pulseWidth--) {
        pwm.setPWM(0, 0, pulseWidth); // Servo 1 (channel 0)
        delay(10);
    }
}
