#include <unity.h>
#include <cmath>

#include <headers/math_implementations.h>

void setUp(void) {
    // set up here
}

void tearDown(void) {
    // tear down here
}

void testCalculateNorm3() {
    BLA::Matrix<3> v;
    v = { 1.0f, 2.0f, 3.0f };
    TEST_ASSERT_EQUAL_FLOAT(sqrt(14), calculateNorm(v));
}

void testIsJacobianSingular() {
    BLA::Matrix<3, 3> J;

    // singular
    J = {
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f
    };

    TEST_ASSERT_TRUE(isJacobianSingular(J));

    // non-singular
    J = {
        5.0f, 3.0f, 9.0f,
        4.0f, 1.0f, 6.0f,
        1.0f, 3.0f, 4.0f
    };

    TEST_ASSERT_FALSE(isJacobianSingular(J));
}

void testInverse3x3Matrix() {
    BLA::Matrix<3,3> A, invertedAExpected;

    A = {
        4.0f, 7.0f, 2.0f,
        3.0f, 6.0f, 1.0f,
        2.0f, 5.0f, 3.0f
    };

    invertedAExpected = {
         1.4444f, -1.2222f, -0.5556f,
        -0.7778f,  0.8889f,  0.2222f,
         0.3333f, -0.6667f,  0.3333f
    };

    BLA::Matrix<3,3> invertedA = BLA::Inverse(A);

    const float TOL = 0.001;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            TEST_ASSERT_FLOAT_WITHIN(TOL, invertedAExpected(i, j), invertedA(i, j));
        }
    }
}

void setup() {
    UNITY_BEGIN();

    RUN_TEST(testCalculateNorm3);
    RUN_TEST(testIsJacobianSingular);
    RUN_TEST(testInverse3x3Matrix);

    UNITY_END();
}

void loop() {

}