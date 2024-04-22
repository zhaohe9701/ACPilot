//
// Created by zhaohe on 2024/4/20.
//

#ifndef ROTARY_TRANS_H_
#define ROTARY_TRANS_H_

#include "Device/device_cali.h"

class RotaryTrans : public DeviceCali
{
public:
    void cali(Vec3 *data, uint16_t num, DeviceCaliData &result) override;
};


#endif //ROTARY_TRANS_H_
