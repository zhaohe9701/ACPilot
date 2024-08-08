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

namespace Component
{

    class Filter
    {
    public:
        virtual void init(float sample_rate, float cutoff_freq) = 0;

        virtual float apply(float sample) = 0;

        virtual void reset() = 0;

        virtual ~Filter() = default;
    };
}

#endif
