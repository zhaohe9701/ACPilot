//
// Created by zhaohe on 2024/4/20.
//

#ifndef BIAS_DETECT_H_
#define BIAS_DETECT_H_

#include "Device/device_cali.h"

class BiasDetect : public DeviceCali
{
    void cali(Vec3 *data, uint16_t num, DeviceCaliData &result) override;
};


#endif //BIAS_DETECT_H_
