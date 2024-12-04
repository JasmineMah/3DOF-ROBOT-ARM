#ifndef ROBOT_ARM
#define ROBOT_ARM

#include <math.h>
#include <algorithm>

struct Point3D {
    double x;
    double y;
    double z;
};

extern const float L1 = 1.2345;
extern const float L2 = 1.2345;
extern const float L3 = 1.2345;

float deg2rad(float theta) {
    return theta * DEG_TO_RAD;
}

float rad2deg(float theta) {
    return theta * RAD_TO_DEG;
}

/// @brief Calculates the forward kinematics of the arm using the Denavit-Hartenberg convention.
/// @param angles 
/// @return The point.
Point3D forward_kinematics(std::vector<double> angles) {
    std::for_each(angles.begin(), angles.end(), &deg2rad);

    double x = L1 * cos(angles[0]) + L2 * cos(angles[0]) * cos(angles[1]) +
              L3 * cos(angles[0]) * cos(angles[1] + angles[2]);

    double y = L1 * sin(angles[0]) + L2 * sin(angles[0]) * cos(angles[1]) +
              L3 * sin(angles[0]) * cos(angles[1] + angles[2]);

    double z = L2 * sin(angles[1]) + L3 * sin(angles[2] + angles[3]);

    return Point3D {x, y, z};
}

#endif