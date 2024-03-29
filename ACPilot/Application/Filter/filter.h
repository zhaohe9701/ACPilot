/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-12 15:30:50
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-22 21:20:12
 * @FilePath: \ZH_FLIGHT\Sys\Flight\filter.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef FILTER_H_
#define FILTER_H_

#define IIR_SHIFT   8

class FirstOrderLPF
{
public:
	void init(float sample_freq, float cutoff_freq);
	float apply(float sample);
	float reset(float sample);

private:
    float _a1 = 0.0f;
	float _a2 = 0.0f;
	float _b0 = 0.0f;
	float _b1 = 0.0f;
	float _b2 = 0.0f;
	float _delay_element_1 = 0.0f;
	float _delay_element_2 = 0.0f;
	void _setCutoffFreq(float sample_freq, float cutoff_freq);
};

class SecondOrderLPF
{
public:
	void init(float sample_freq, float cutoff_freq);
	float apply(float sample);
	float reset(float sample);

private:
    float _alpha = 0.0f;
	float _pre_output = 0.0f;
	void _setCutoffFreq(float sample_freq, float cutoff_freq);
};
#endif
