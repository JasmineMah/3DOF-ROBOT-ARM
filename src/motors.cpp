#include "headers/motors.h"

/// @brief Moves all three motors by angles `delta_angles`.
/// @param pwm PWM servo driver instance.
/// @param delta_angles Angles for which to move the motors by.
void moveMotors(Adafruit_PWMServoDriver &pwm, BLA::Matrix<3> delta_angles) {
    for (int i = 0; i < delta_angles.Cols; i++) {
        // Ideally if time permits, some kind of threading would be used here.
        moveMotor(pwm, i, delta_angles(i));
    }
}

/// @brief Moves an individual motor.
/// @param pwm PWM servo driver instance.
/// @param motor_no Motor number.
/// @param degrees Degrees to offset.
void moveMotor(Adafruit_PWMServoDriver &pwm, int motor_no, float degrees) {
    // TODO: move one individual motor by `degrees`.
    // Need more information on converting input degrees to encoder positions.
}

/// @brief Gets motor angles from the encoders.
/// @return Motor angles.
BLA::Matrix<3> getMotorAngles() {
    return BLA::Matrix<3> {base, elbow, wrist};
}

/// @brief Homes a singular motor.
/// @param pwm PWM controller instance.
/// @param encoder Corresponding encoder instance to the motor.
/// @param indexPin Index pin of the encoder.
/// @param pwmChannel Motor channel number. Identifies the motor number.
void homeMotor(Adafruit_PWMServoDriver &pwm, Encoder& encoder, int indexPin, int pwmChannel) {
    pinMode(indexPin, INPUT);

    // Rotate motor slowly in one direction
    pwm.setPWM(pwmChannel, 0, SERVOMIN);
    Serial.println("Searching for index");
    while (digitalRead(indexPin) == LOW) {
        Serial.print(".");
        delay(50);
    }
    
    // Stop motor, and reset encoder position
    pwm.setPWM(pwmChannel, 0, 0);
    encoder.write(0);

    delay(1000);
}

/// @brief Toggles grab state of the last motor.
/// @param pwm PWM controller instance.
/// @param grab Whether or not to release.
void toggleGrab(Adafruit_PWMServoDriver &pwm, bool grab) {
    // TODO: CLAMP DOWN OR RELEASE THE END EFFECTOR MOTOR DEPENDING ON THE VALUE OF `grab`
}