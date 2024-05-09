//
// Created by zhaohe on 2024/4/29.
//

#include "lpf.h"
#include "default_debug.h"
#include <math.h>
#define PI 3.1415927f

void SecondOrderLPF::init(float sample_freq, float cutoff_freq)
{
    _sample_freq = sample_freq;
    _cutoff_freq = cutoff_freq;
    float fr = sample_freq / cutoff_freq;
    float ohm = tanf(PI / fr);
    float c = 1.0f + 2.0f * cosf(PI / 4.0f) * ohm + ohm * ohm;
    _b0 = ohm * ohm / c;
    _b1 = 2.0f * _b0;
    _b2 = _b0;
    _a1 = 2.0f * (ohm * ohm - 1.0f) / c;
    _a2 = (1.0f - 2.0f * cosf(PI / 4.0f) * ohm + ohm * ohm) / c;
    _delay_element_1 = 0.0f;
    _delay_element_2 = 0.0f;
}

float SecondOrderLPF::apply(float sample)
{
    float delay_element_0 = sample - _delay_element_1 * _a1 - _delay_element_2 * _a2;
    if (!isfinite(delay_element_0))
    {
        delay_element_0 = sample;
    }

    float output = delay_element_0 * _b0 + _delay_element_1 * _b1 + _delay_element_2 * _b2;

    _delay_element_2 = _delay_element_1;
    _delay_element_1 = delay_element_0;
    return output;
}

void SecondOrderLPF::reset()
{
    float fr = _sample_freq / _cutoff_freq;
    float ohm = tanf(PI / fr);
    float c = 1.0f + 2.0f * cosf(PI / 4.0f) * ohm + ohm * ohm;
    _b0 = ohm * ohm / c;
    _b1 = 2.0f * _b0;
    _b2 = _b0;
    _a1 = 2.0f * (ohm * ohm - 1.0f) / c;
    _a2 = (1.0f - 2.0f * cosf(PI / 4.0f) * ohm + ohm * ohm) / c;
    _delay_element_1 = 0.0f;
    _delay_element_2 = 0.0f;
}



void FirstOrderLPF::init(float sample_freq, float cutoff_freq)
{
    _sample_freq = sample_freq;
    _cutoff_freq = cutoff_freq;
    _a1 = expf(- 2.0f * PI * cutoff_freq / sample_freq);
    _a2 = 1.0f - _a1;
}

float FirstOrderLPF::apply(float sample)
{
    float output = _a1 * _pre_output + _a2 * sample;
    _pre_output = output;
    return output;
}

void FirstOrderLPF::reset()
{
    _a1 = expf(- 2.0f * PI * _cutoff_freq / _sample_freq);
    _a2 = 1.0f - _a1;
}


