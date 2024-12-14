#include <Encoder.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Adafruit_PWMServoDriver.h>

// Encoder 1
#define ENCODER1_A 32
#define ENCODER1_B 4
#define ENCODER1_INDEX 13 

// Encoder 2
#define ENCODER2_A 16
#define ENCODER2_B 17
#define ENCODER2_INDEX 18

// Encoder 3
#define ENCODER3_A 19
#define ENCODER3_B 21
#define ENCODER3_INDEX 15

// LCD pins
#define LCD_RS 14
#define LCD_EN 12
#define LCD_D4 27
#define LCD_D5 26
#define LCD_D6 25
#define LCD_D7 33

// Servo pulse range
#define SERVOMIN 150  // Minimum pulse length for homing
#define SERVOMAX 150  // Maximum pulse length for slow motor movement

// PCA9685 setup
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Encoders and LCD
Encoder myEncoder1(ENCODER1_A, ENCODER1_B);
Encoder myEncoder2(ENCODER2_A, ENCODER2_B);
Encoder myEncoder3(ENCODER3_A, ENCODER3_B);
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup() {
    Serial.begin(115200);
    lcd.begin(16, 2);
    lcd.print("Initializing...");
    delay(1000);

    // Initialize PCA9685
    Wire.begin();
    pwm.begin();
    pwm.setPWMFreq(50); // Frequency for servos (50Hz)
    delay(10);

    lcd.clear();
    lcd.print("Homing Motors...");
    delay(1000);

    // Homing sequence for each motor
    homeMotor("Motor 1", myEncoder1, ENCODER1_INDEX, 0);
    homeMotor("Motor 2", myEncoder2, ENCODER2_INDEX, 1);
    homeMotor("Motor 3", myEncoder3, ENCODER3_INDEX, 2);

    lcd.clear();
    lcd.print("Homing Done!");
    delay(2000);
    lcd.clear();
}

void loop() {
    // Get encoder positions
    long position1 = myEncoder1.read();
    long position2 = myEncoder2.read();
    long position3 = myEncoder3.read();

    // Display positions on LCD
    lcd.setCursor(0, 0);
    lcd.print("M1:");
    lcd.print(position1);
    lcd.print(" M2:");
    lcd.print(position2);
    lcd.setCursor(0, 1);
    lcd.print("M3:");
    lcd.print(position3);

    // Debugging messages
    Serial.print("Motor 1 Position: ");
    Serial.println(position1);
    Serial.print("Motor 2 Position: ");
    Serial.println(position2);
    Serial.print("Motor 3 Position: ");
    Serial.println(position3);
    Serial.println("----------------------");

    delay(500);
}

void homeMotor(const char* motorName, Encoder& encoder, int indexPin, int pwmChannel) {
    pinMode(indexPin, INPUT);

    lcd.clear();
    lcd.print(motorName);
    lcd.setCursor(0, 1);
    lcd.print("Homing...");

    Serial.print(motorName);
    Serial.println(": Starting homing sequence...");

    // Rotate motor slowly in one direction
    pwm.setPWM(pwmChannel, 0, SERVOMIN);
    while (digitalRead(indexPin) == LOW) {
        Serial.print(motorName);
        Serial.println(": Searching for Index...");
        delay(50); // Small delay to check the index pin
    }

    // Stop motor and reset encoder position
    pwm.setPWM(pwmChannel, 0, 0); // Stop motor
    encoder.write(0); // Reset encoder to zero

    lcd.setCursor(0, 1);
    lcd.print("Done.");
    delay(1000); // Brief pause before continuing

    Serial.print(motorName);
    Serial.println(": Homing complete.");
}
