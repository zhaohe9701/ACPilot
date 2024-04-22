//
// Created by zhaohe on 2024/4/20.
//
#include <math.h>
#include "rotary_trans.h"

void RotaryTrans::cali(Vec3 *data, uint16_t num, DeviceCaliData &result)
{
    float absxk;
    float b_y;
    float pf_idx_0;
    float pf_idx_1;
    float pr_idx_0;
    float pr_idx_1;
    float pr_idx_2;
    float scale;
    float t;
    float t0_idx_0;
    float t0_idx_1;
    float t0_idx_2;
    float t1_idx_0;
    float t1_idx_1;
    float t1_idx_2;
    float y;
    scale = 1.29246971E-26F;
    absxk = (float)fabs(data[0].x);
    if (absxk > 1.29246971E-26F) {
        y = 1.0F;
        scale = absxk;
    } else {
        t = absxk / 1.29246971E-26F;
        y = t * t;
    }
    absxk = (float)fabs(data[0].y);
    if (absxk > scale) {
        t = scale / absxk;
        y = y * t * t + 1.0F;
        scale = absxk;
    } else {
        t = absxk / scale;
        y += t * t;
    }
    absxk = (float)fabs(data[0].z);
    if (absxk > scale) {
        t = scale / absxk;
        y = y * t * t + 1.0F;
        scale = absxk;
    } else {
        t = absxk / scale;
        y += t * t;
    }
    y = scale * (float)sqrt(y);
    scale = 1.29246971E-26F;
    t0_idx_0 = data[0].x / y;
    absxk = (float)fabs(data[1].x);
    if (absxk > 1.29246971E-26F) {
        b_y = 1.0F;
        scale = absxk;
    } else {
        t = absxk / 1.29246971E-26F;
        b_y = t * t;
    }
    t0_idx_1 = data[0].y / y;
    absxk = (float)fabs(data[1].y);
    if (absxk > scale) {
        t = scale / absxk;
        b_y = b_y * t * t + 1.0F;
        scale = absxk;
    } else {
        t = absxk / scale;
        b_y += t * t;
    }
    t0_idx_2 = data[0].z / y;
    absxk = (float)fabs(data[1].z);
    if (absxk > scale) {
        t = scale / absxk;
        b_y = b_y * t * t + 1.0F;
        scale = absxk;
    } else {
        t = absxk / scale;
        b_y += t * t;
    }
    b_y = scale * (float)sqrt(b_y);
    t1_idx_0 = data[1].x / b_y;
    t1_idx_1 = data[1].y / b_y;
    t1_idx_2 = data[1].z / b_y;
    pr_idx_0 = t0_idx_1 * t1_idx_2 - t1_idx_1 * t0_idx_2;
    pr_idx_1 = t1_idx_0 * t0_idx_2 - t0_idx_0 * t1_idx_2;
    pr_idx_2 = t0_idx_0 * t1_idx_1 - t1_idx_0 * t0_idx_1;
    pf_idx_0 = t0_idx_1 * pr_idx_2 - pr_idx_1 * t0_idx_2;
    pf_idx_1 = pr_idx_0 * t0_idx_2 - t0_idx_0 * pr_idx_2;
    t0_idx_2 = t0_idx_0 * pr_idx_1 - pr_idx_0 * t0_idx_1;
    t1_idx_0 = pr_idx_1 * t0_idx_2 - pf_idx_1 * pr_idx_2;
    t1_idx_1 = pf_idx_0 * pr_idx_2 - pr_idx_0 * t0_idx_2;
    t1_idx_2 = pr_idx_0 * pf_idx_1 - pf_idx_0 * pr_idx_1;
    scale = 1.29246971E-26F;
    absxk = (float)fabs(pr_idx_0);
    if (absxk > 1.29246971E-26F) {
        y = 1.0F;
        scale = absxk;
    } else {
        t = absxk / 1.29246971E-26F;
        y = t * t;
    }
    absxk = (float)fabs(pr_idx_1);
    if (absxk > scale) {
        t = scale / absxk;
        y = y * t * t + 1.0F;
        scale = absxk;
    } else {
        t = absxk / scale;
        y += t * t;
    }
    absxk = (float)fabs(pr_idx_2);
    if (absxk > scale) {
        t = scale / absxk;
        y = y * t * t + 1.0F;
        scale = absxk;
    } else {
        t = absxk / scale;
        y += t * t;
    }
    y = scale * (float)sqrt(y);
    scale = 1.29246971E-26F;
    pr_idx_0 /= y;
    absxk = (float)fabs(pf_idx_0);
    if (absxk > 1.29246971E-26F) {
        b_y = 1.0F;
        scale = absxk;
    } else {
        t = absxk / 1.29246971E-26F;
        b_y = t * t;
    }
    pr_idx_1 /= y;
    absxk = (float)fabs(pf_idx_1);
    if (absxk > scale) {
        t = scale / absxk;
        b_y = b_y * t * t + 1.0F;
        scale = absxk;
    } else {
        t = absxk / scale;
        b_y += t * t;
    }
    pr_idx_2 /= y;
    absxk = (float)fabs(t0_idx_2);
    if (absxk > scale) {
        t = scale / absxk;
        b_y = b_y * t * t + 1.0F;
        scale = absxk;
    } else {
        t = absxk / scale;
        b_y += t * t;
    }
    b_y = scale * (float)sqrt(b_y);
    scale = 1.29246971E-26F;
    pf_idx_0 /= b_y;
    absxk = (float)fabs(t1_idx_0);
    if (absxk > 1.29246971E-26F) {
        y = 1.0F;
        scale = absxk;
    } else {
        t = absxk / 1.29246971E-26F;
        y = t * t;
    }
    pf_idx_1 /= b_y;
    absxk = (float)fabs(t1_idx_1);
    if (absxk > scale) {
        t = scale / absxk;
        y = y * t * t + 1.0F;
        scale = absxk;
    } else {
        t = absxk / scale;
        y += t * t;
    }
    t0_idx_2 /= b_y;
    absxk = (float)fabs(t1_idx_2);
    if (absxk > scale) {
        t = scale / absxk;
        y = y * t * t + 1.0F;
        scale = absxk;
    } else {
        t = absxk / scale;
        y += t * t;
    }
    y = scale * (float)sqrt(y);
    result.rot[0][0] = pr_idx_0;
    result.rot[0][1] = pf_idx_0;
    result.rot[0][2] = t1_idx_0 / y;
    result.rot[1][0] = pr_idx_1;
    result.rot[1][1] = pf_idx_1;
    result.rot[1][2] = t1_idx_1 / y;
    result.rot[2][0] = pr_idx_2;
    result.rot[2][1] = t0_idx_2;
    result.rot[2][2] = t1_idx_2 / y;
}
