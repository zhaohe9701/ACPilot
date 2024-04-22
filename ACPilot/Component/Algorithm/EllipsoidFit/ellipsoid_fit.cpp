//
// Created by zhaohe on 2024/4/17.
//

#include <math.h>
#include <string.h>
#include "ellipsoid_fit.h"
#include "os.h"

EllipsoidFit::EllipsoidFit(float unit)
{
    _unit = unit;
}

void EllipsoidFit::cali(Vec3 *data, uint16_t num, DeviceCaliData &result)
{
    A_tmp = new float[6 * num];
    D = new float[6 * num];
    float A[36];
    float v[6];
    float f;
    float s;
    float smax;
    int a;
    int b_A_tmp;
    int b_tmp;
    int i;
    int j;
    int jA;
    int jp1j;
    int k;
    int mmj_tmp;
    signed char ipiv[6];
    signed char i1;
    for (i = 0; i < num; i++) {
        smax = data[i].x;
        D[i] = smax * smax;
        s = data[i].y;
        D[i + num] = s * s;
        f = data[i].z;
        D[i + 2 * num] = f * f;
        D[i + 3 * num] = 2.0F * smax;
        D[i + 4 * num] = 2.0F * s;
        D[i + 5 * num] = 2.0F * f;
        for (jp1j = 0; jp1j < 6; jp1j++) {
            A_tmp[jp1j + 6 * i] = D[i + num * jp1j];
        }
    }
    for (i = 0; i < 6; i++) {
        for (jp1j = 0; jp1j < 6; jp1j++) {
            smax = 0.0F;
            for (jA = 0; jA < num; jA++) {
                smax += A_tmp[i + 6 * jA] * D[jA + num * jp1j];
            }
            A[i + 6 * jp1j] = smax;
        }
        smax = 0.0F;
        for (jp1j = 0; jp1j < num; jp1j++) {
            smax += A_tmp[i + 6 * jp1j];
        }
        v[i] = smax;
        ipiv[i] = (signed char)(i + 1);
    }
    for (j = 0; j < 5; j++) {
        mmj_tmp = 4 - j;
        b_tmp = j * 7;
        jp1j = b_tmp + 2;
        jA = 6 - j;
        a = 0;
        smax = (float)fabs(A[b_tmp]);
        for (k = 2; k <= jA; k++) {
            s = (float)fabs(A[(b_tmp + k) - 1]);
            if (s > smax) {
                a = k - 1;
                smax = s;
            }
        }
        if (A[b_tmp + a] != 0.0F) {
            if (a != 0) {
                jA = j + a;
                ipiv[j] = (signed char)(jA + 1);
                for (k = 0; k < 6; k++) {
                    a = j + k * 6;
                    smax = A[a];
                    b_A_tmp = jA + k * 6;
                    A[a] = A[b_A_tmp];
                    A[b_A_tmp] = smax;
                }
            }
            i = (b_tmp - j) + 6;
            for (a = jp1j; a <= i; a++) {
                A[a - 1] /= A[b_tmp];
            }
        }
        jA = b_tmp;
        for (a = 0; a <= mmj_tmp; a++) {
            smax = A[(b_tmp + a * 6) + 6];
            if (smax != 0.0F) {
                i = jA + 8;
                jp1j = (jA - j) + 12;
                for (b_A_tmp = i; b_A_tmp <= jp1j; b_A_tmp++) {
                    A[b_A_tmp - 1] += A[((b_tmp + b_A_tmp) - jA) - 7] * -smax;
                }
            }
            jA += 6;
        }
        i1 = ipiv[j];
        if (i1 != j + 1) {
            smax = v[j];
            v[j] = v[i1 - 1];
            v[i1 - 1] = smax;
        }
    }
    for (k = 0; k < 6; k++) {
        jA = 6 * k;
        if (v[k] != 0.0F) {
            i = k + 2;
            for (a = i; a < 7; a++) {
                v[a - 1] -= v[k] * A[(a + jA) - 1];
            }
        }
    }
    for (k = 5; k >= 0; k--) {
        jA = 6 * k;
        smax = v[k];
        if (smax != 0.0F) {
            smax /= A[k + jA];
            v[k] = smax;
            for (a = 0; a < k; a++) {
                v[a] -= v[k] * A[a + jA];
            }
        }
    }
    /*  最小二乘拟合  */
    /*  偏移是椭球的中心 */
    smax =
            ((v[3] * v[3] / v[0] + v[4] * v[4] / v[1]) + v[5] * v[5] / v[2]) + 1.0F;

    result.offset.x = -(v[3] / v[0]);
    result.gain.x = _unit / sqrt(smax / v[0]);
    result.offset.y = -(v[4] / v[1]);
    result.gain.y = _unit / sqrt(smax / v[1]);
    result.offset.z = -(v[5] / v[2]);
    result.gain.z = _unit / sqrt(smax / v[2]);

    /*  找到椭球的半径（比例） */
    delete[] A_tmp;
    delete[] D;
}








