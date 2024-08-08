/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-24 17:52:30
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-19 23:51:02
 * @FilePath: \ZH_FLIGHT\Sys\AHRS\mahony.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef MAHONY_H_
#define MAHONY_H_

#include "AHRS/attitude_solver.h"

namespace Component
{

    class Mahony : public AttitudeSolver
    {
    public:
        void update(MemsData &mems, Common::Euler &euler, float dt) override;

        void reset() override;

    private:
        static float _invSqrt(float x);

        void _computeRotationMatrix();

        float _kp = 0.4f;
        float _ki = 0.001f;
        Common::Vec3 _e_int;

        Common::Vec4 _q{1.0f, 0.0f, 0.0f, 0.0f};
    };
}
#endif
