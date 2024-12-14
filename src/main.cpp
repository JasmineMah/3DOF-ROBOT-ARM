#include <Wire.h>
#include <SPI.h>
#include <LiquidCrystal.h>

#include "headers/motors.h"
#include "headers/webserver.h"

#define DISABLE_WEBSERVER_STARTUP_LOOP // We aren't debugging the webserver standalone.

// LCD pins
#define LCD_RS 14
#define LCD_EN 12
#define LCD_D4 27
#define LCD_D5 26
#define LCD_D6 25
#define LCD_D7 33

// PCA9685 setup
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Encoders and LCD
Encoder myEncoder1(ENCODER1_A, ENCODER1_B);
Encoder myEncoder2(ENCODER2_A, ENCODER2_B);
Encoder myEncoder3(ENCODER3_A, ENCODER3_B);
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup() {
    Serial.begin(115200);
    lcd.begin(16,2);

    // Begin the web server
    initWebServer();

    // On each HTTP request, if time permits, we would handle it with 


}

void loop() {
    handleWebServer();

    // Continually read encoder values from the motors.
}