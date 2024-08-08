//
// Created by zhaohe on 2024/4/17.
//
#include "attitude_solver.h"

using namespace Component;

void AttitudeSolver::transformBodyToEarth(Common::Vec3 &vec)
{
    Common::Vec3 e;
    e.x = vec.x * _r_mat[0][0] + vec.y * _r_mat[0][1] + vec.z * _r_mat[0][2];
    e.y = vec.x * _r_mat[1][0] + vec.y * _r_mat[1][1] + vec.z * _r_mat[1][2];
    e.z = vec.x * _r_mat[2][0] + vec.y * _r_mat[2][1] + vec.z * _r_mat[2][2];

    vec.x = e.x;
    vec.y = e.y;
    vec.z = e.z;
}

void AttitudeSolver::transformEarthToBody(Common::Vec3 &vec)
{
    Common::Vec3 b;

    b.x = vec.x * _r_mat[0][0] + vec.y * _r_mat[1][0] + vec.z * _r_mat[2][0];
    b.y = vec.x * _r_mat[0][1] + vec.y * _r_mat[1][1] + vec.z * _r_mat[2][1];
    b.z = vec.x * _r_mat[0][2] + vec.y * _r_mat[1][2] + vec.z * _r_mat[2][2];

    vec.x = b.x;
    vec.y = b.y;
    vec.z = b.z;
}

