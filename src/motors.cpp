#include <motors.h>

/// @brief Moves an individual motor.
/// @param pwm PWM servo driver instance.
/// @param motor_no Motor number.
/// @param degrees Degrees to offset.
void moveMotor(Adafruit_PWMServoDriver &pwm, uint8_t motor_no, float degrees) {
    // TODO: move one individual motor by `degrees`
}

/// @brief Moves all three motors by angles `delta_angles`.
/// @param pwm PWM servo driver instance.
/// @param delta_angles Angles for which to move the motors by.
void moveMotors(Adafruit_PWMServoDriver &pwm, BLA::Matrix<3> delta_angles) {
    for (int i = 0; i < delta_angles.Cols; i++) {
        // Ideally if time permits, threading would be used here.
        moveMotor(pwm, i, delta_angles(i));
    }
}

/// @brief Gets motor angles from the encoders.
/// @return Motor angles.
BLA::Matrix<3> getMotorAngles() {
    // Waiting on encoder support and interfacing.
}

// TODO: Implement moveToPosition() in the similar way that we would call move_to_position() for the LEGO motors.