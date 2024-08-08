//
// Created by zhaohe on 2024/4/20.
//

#ifndef BIAS_DETECT_H_
#define BIAS_DETECT_H_

#include "Device/device_cali.h"

namespace Component
{
    class BiasDetect : public Framework::DeviceCali
    {
        void cali(Common::Vec3 *data, uint16_t num, Framework::DeviceCaliData &result) override;
    };
}

#endif //BIAS_DETECT_H_
