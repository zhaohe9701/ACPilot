//
// Created by zhaohe on 2024/4/20.
//

#ifndef ROTARY_TRANS_H_
#define ROTARY_TRANS_H_

#include "Device/device_cali.h"

namespace Component
{
    class RotaryTrans : public Framework::DeviceCali
    {
    public:
        void cali(Common::Vec3 *data, uint16_t num, Framework::DeviceCaliData &result) override;
    };
}

#endif //ROTARY_TRANS_H_
