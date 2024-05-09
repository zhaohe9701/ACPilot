//
// Created by zhaohe on 2024/4/29.
//

#ifndef LPF_H_
#define LPF_H_

#include "Filter/filter.h"

class SecondOrderLPF : public Filter
{
public:
    void init(float sample_freq, float cutoff_freq) override;

    float apply(float sample) override;

    void reset() override;

private:
    float _a1 = 0.0f;
    float _a2 = 0.0f;
    float _b0 = 0.0f;
    float _b1 = 0.0f;
    float _b2 = 0.0f;
    float _delay_element_1 = 0.0f;
    float _delay_element_2 = 0.0f;
    float _sample_freq = 0.0f;
    float _cutoff_freq = 0.0f;
};

class FirstOrderLPF : public Filter
{
public:
    void init(float sample_freq, float cutoff_freq) override;

    float apply(float sample) override;

    void reset() override;

private:
    float _a1 = 0.0f;
    float _a2 = 0.0f;
    float _pre_output = 0.0f;
    float _sample_freq = 0.0f;
    float _cutoff_freq = 0.0f;
};

#endif //LPF_H_
