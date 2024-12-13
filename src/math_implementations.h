#ifndef MATH_IMPLEMENTATIONS_H
#define MATH_IMPLEMENTATIONS_H

#include <cmath>
#include <BasicLinearAlgebra.h>

/// @brief Calculates the norm of a 1x3 vector.
/// @param v 
/// @return The norm.
float calculateNorm(BLA::Matrix<3> v) {
    BLA::Matrix<1> inner = ~v * v;
    return sqrt(inner(0));
}

/// @brief Checks if the Jacobian is singular.
/// @param J Input Jacobian.
/// @return Whether or not the Jacobian is singular.
bool isJacobianSingular(const BLA::Matrix<3,3>& J) {
    double determinant =
        J(0, 0) * (J(1, 1) * J(2, 2) - J(1, 2) * J(2, 1)) -
        J(0, 1) * (J(1, 0) * J(2, 2) - J(1, 2) * J(2, 0)) +
        J(0, 2) * (J(1, 0) * J(2, 1) - J(1, 1) * J(2, 0));

    const double EPSILON = 1e-3;
    return std::abs(determinant) < EPSILON;
}

#endif