#include <Wire.h>
#include <SPI.h>
#include <LiquidCrystal.h>

#include "headers/motors.h"
#include "headers/webserver.h"
#include "headers/robot_arm.h"

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
Encoder enc1(ENCODER1_A, ENCODER1_B);
Encoder enc2(ENCODER2_A, ENCODER2_B);
Encoder enc3(ENCODER3_A, ENCODER3_B);
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup() {
    Serial.begin(115200);
    lcd.begin(16, 2);

    // Home our motors to get a proper reference.
    lcd.print("Initializing...");
    homeMotor(pwm, enc1, ENCODER1_INDEX, 0);
    homeMotor(pwm, enc2, ENCODER2_INDEX, 1);
    homeMotor(pwm, enc3, ENCODER3_INDEX, 2);
    
    lcd.clear();
    lcd.print("Homing complete.");
    delay(2000);

    // Start the web server.
    initWebServer();

    // On each HTTP request, if time permits, we would handle it with an interrupt, supported by an OS (FreeRTOS / ROS...)
    // OR we poll for when the user submits the form.
    // handleCommand();

}

/// @brief Executes forward or inverse kinematics when the HTML client sends back values.
/// Here we unpack the values as needed.
void handleCommand() {
    if (useXYZ) {
        BLA::Matrix<3> targetPoint = {v0, v1, v2};
        bool movementSuccessful = newton(pwm, targetPoint, 50, 2.5f);

    } else {
        BLA::Matrix<3> targetAngles = {v0, v1, v2};
        BLA::Matrix<3> deltaAngles = targetAngles - BLA::Matrix<3>{base, elbow, wrist};
        moveMotors(pwm, deltaAngles);
    }
    
    toggleGrab(pwm, grab);
}

float convertEncoderReadingToAngle(long rawEncoderReading) {
    // TODO: Convert raw encoder readings to joint angles that we can work with.
    // Need more information on converting input degrees to encoder positions.
    return (float) rawEncoderReading;
}

#ifdef DISABLE_WEBSERVER_STANDALONE
void loop() {
    // Listens for the incoming client.
    handleWebServer();

    // Continuously read in data which will presented to the web server.
    long pos1 = enc1.read();
    long pos2 = enc2.read();
    long pos3 = enc3.read();

    base = convertEncoderReadingToAngle(pos1);
    elbow = convertEncoderReadingToAngle(pos2);
    wrist = convertEncoderReadingToAngle(pos3);

    BLA::Matrix<3> endEffectorXYZ = forwardKinematics(BLA::Matrix<3>{base, elbow, wrist});

    X = endEffectorXYZ(0);
    Y = endEffectorXYZ(1);
    Z = endEffectorXYZ(2);
}
#endif