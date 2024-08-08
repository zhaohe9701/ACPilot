//
// Created by zhaohe on 2024/5/9.
//

#include <math.h>
#include "altitude_estimate.h"

using namespace Component;

AC_RET AltitudeEstimate::init(float r00, float r11, float q00, float q11, float q22)
{
    R[0] = r00;
    R[3] = r11;
    Q[0] = q00;
    Q[4] = q11;
    Q[8] = q22;

    F[0] = 1;
    F[1] = 0;
    F[2] = 0;
    F[4] = 1;
    F[5] = 0;
    F[8] = 1;

    H[0] = 1;
    H[1] = 0;
    H[2] = 0;
    H[3] = 0;
    H[4] = 0;
    H[5] = 1;

    P[0] = 1;
    P[4] = 1;
    P[8] = 1;

    return AC_OK;
}

AC_RET AltitudeEstimate::calculate(const float altitude, const float acc, float X[3], float dt)
{
    F[3] = dt;
    F[6] = 0.5f * dt * dt;
    F[7] = dt;

    float Z[2] = {altitude, acc};
    float P1[9];
    float b_F[9];
    float K[6];
    float b_H[6];
    float y[6];
    float B[4];
    float Xn[3];
    float b_Z[2];
    float a21;
    float a22;
    float a22_tmp;
    int K_tmp;
    int b_K_tmp;
    int r1;
    int r2;
    signed char b_I[9];
    for (K_tmp = 0; K_tmp < 3; K_tmp++) {
        a21 = 0.0F;
        for (b_K_tmp = 0; b_K_tmp < 3; b_K_tmp++) {
            r1 = K_tmp + 3 * b_K_tmp;
            a21 += F[r1] * X0[b_K_tmp];
            b_F[r1] =
                    (F[K_tmp] * P[3 * b_K_tmp] + F[K_tmp + 3] * P[3 * b_K_tmp + 1]) +
                    F[K_tmp + 6] * P[3 * b_K_tmp + 2];
        }
        Xn[K_tmp] = a21;
        a21 = b_F[K_tmp];
        a22_tmp = b_F[K_tmp + 3];
        a22 = b_F[K_tmp + 6];
        for (b_K_tmp = 0; b_K_tmp < 3; b_K_tmp++) {
            r1 = K_tmp + 3 * b_K_tmp;
            P1[r1] = ((a21 * F[b_K_tmp] + a22_tmp * F[b_K_tmp + 3]) +
                      a22 * F[b_K_tmp + 6]) +
                     Q[r1];
        }
    }
    for (K_tmp = 0; K_tmp < 2; K_tmp++) {
        K[3 * K_tmp] = H[K_tmp];
        K[3 * K_tmp + 1] = H[K_tmp + 2];
        K[3 * K_tmp + 2] = H[K_tmp + 4];
    }
    for (K_tmp = 0; K_tmp < 3; K_tmp++) {
        a21 = P1[K_tmp];
        a22_tmp = P1[K_tmp + 3];
        a22 = P1[K_tmp + 6];
        for (b_K_tmp = 0; b_K_tmp < 2; b_K_tmp++) {
            y[K_tmp + 3 * b_K_tmp] =
                    (a21 * K[3 * b_K_tmp] + a22_tmp * K[3 * b_K_tmp + 1]) +
                    a22 * K[3 * b_K_tmp + 2];
        }
    }
    for (K_tmp = 0; K_tmp < 2; K_tmp++) {
        a21 = H[K_tmp];
        a22_tmp = H[K_tmp + 2];
        a22 = H[K_tmp + 4];
        for (b_K_tmp = 0; b_K_tmp < 3; b_K_tmp++) {
            b_H[K_tmp + (b_K_tmp << 1)] =
                    (a21 * P1[3 * b_K_tmp] + a22_tmp * P1[3 * b_K_tmp + 1]) +
                    a22 * P1[3 * b_K_tmp + 2];
        }
        a21 = b_H[K_tmp];
        a22_tmp = b_H[K_tmp + 2];
        a22 = b_H[K_tmp + 4];
        for (b_K_tmp = 0; b_K_tmp < 2; b_K_tmp++) {
            r1 = K_tmp + (b_K_tmp << 1);
            B[r1] = ((a21 * K[3 * b_K_tmp] + a22_tmp * K[3 * b_K_tmp + 1]) +
                     a22 * K[3 * b_K_tmp + 2]) +
                    R[r1];
        }
    }
    if ((float)fabs(B[1]) > (float)fabs(B[0])) {
        r1 = 1;
        r2 = 0;
    } else {
        r1 = 0;
        r2 = 1;
    }
    a21 = B[r2] / B[r1];
    a22_tmp = B[r1 + 2];
    a22 = B[r2 + 2] - a21 * a22_tmp;
    K[3 * r1] = y[0] / B[r1];
    K[3 * r2] = (y[3] - K[3 * r1] * a22_tmp) / a22;
    K[3 * r1] -= K[3 * r2] * a21;
    K_tmp = 3 * r1 + 1;
    K[K_tmp] = y[1] / B[r1];
    b_K_tmp = 3 * r2 + 1;
    K[b_K_tmp] = (y[4] - K[K_tmp] * a22_tmp) / a22;
    K[K_tmp] -= K[b_K_tmp] * a21;
    K_tmp = 3 * r1 + 2;
    K[K_tmp] = y[2] / B[r1];
    b_K_tmp = 3 * r2 + 2;
    K[b_K_tmp] = (y[5] - K[K_tmp] * a22_tmp) / a22;
    K[K_tmp] -= K[b_K_tmp] * a21;
    a21 = Xn[0];
    a22_tmp = Xn[1];
    a22 = Xn[2];
    for (K_tmp = 0; K_tmp < 2; K_tmp++) {
        b_Z[K_tmp] = Z[K_tmp] - ((H[K_tmp] * a21 + H[K_tmp + 2] * a22_tmp) +
                                 H[K_tmp + 4] * a22);
    }
    a21 = b_Z[0];
    a22_tmp = b_Z[1];
    for (K_tmp = 0; K_tmp < 3; K_tmp++) {
        X[K_tmp] = Xn[K_tmp] + (K[K_tmp] * a21 + K[K_tmp + 3] * a22_tmp);
    }
    for (K_tmp = 0; K_tmp < 9; K_tmp++) {
        b_I[K_tmp] = 0;
    }
    b_I[0] = 1;
    b_I[4] = 1;
    b_I[8] = 1;
    for (K_tmp = 0; K_tmp < 3; K_tmp++) {
        a21 = K[K_tmp];
        a22_tmp = K[K_tmp + 3];
        for (b_K_tmp = 0; b_K_tmp < 3; b_K_tmp++) {
            r1 = b_K_tmp << 1;
            r2 = K_tmp + 3 * b_K_tmp;
            b_F[r2] = (float)b_I[r2] - (a21 * H[r1] + a22_tmp * H[r1 + 1]);
        }
        a21 = b_F[K_tmp];
        a22_tmp = b_F[K_tmp + 3];
        a22 = b_F[K_tmp + 6];
        for (b_K_tmp = 0; b_K_tmp < 3; b_K_tmp++) {
            P[K_tmp + 3 * b_K_tmp] =
                    (a21 * P1[3 * b_K_tmp] + a22_tmp * P1[3 * b_K_tmp + 1]) +
                    a22 * P1[3 * b_K_tmp + 2];
        }
    }
    X0[0] = X[0];
    X0[1] = X[1];
    X0[2] = X[2];
    return AC_OK;
}

void AltitudeEstimate::clear()
{
    P[0] = 1;
    P[4] = 1;
    P[8] = 1;
}
