//
// Created by zhaohe on 2024/4/29.
//

#ifndef PID_H_
#define PID_H_

#include "Filter/filter.h"

namespace Component
{
    class Proportion
    {
    public:
        void init(float gain);

        void reset(float gain);

        float calculate(float error);

    private:
        float _gain = 1.0f;
    };

    class Integral
    {
    public:
        void init(float gain, float limit);

        void reset(float gain, float limit);

        void clear();

        float calculate(float error, float dt);

    private:
        float _gain = 0.0f;
        float _limit = 1.0f;
        float _sum = 0.0f;
    };

    class Differential
    {
    public:
        void init(float gain);

        void reset(float gain);

        float calculate(float error, float dt);

        float getGain();

    private:
        float _gain = 0.0f;
        float _last_error = 0.0f;
    };


    class Pid
    {
    public:
        Pid() = default;

        explicit Pid(Filter *diff_filter);

        void init(float kp, float ki, float kd, float out_limit, float int_limit);

        float output(float error, float d_error, float dt);

        float output(float error, float dt);

        void reset(float kp, float ki, float kd, float out_limit, float int_limit);

        void clear();

    private:
        Proportion _proportion;
        Integral _integral;
        Differential _differential;
        Filter *_diff_filter = nullptr;
        float _out_limit = 0.0f;
    };
}
#endif //PID_H_
