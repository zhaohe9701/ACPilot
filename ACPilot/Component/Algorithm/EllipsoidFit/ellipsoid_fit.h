//
// Created by zhaohe on 2024/4/17.
//

#ifndef ELLIPSOID_FIT_H_
#define ELLIPSOID_FIT_H_

#include "Device/device_cali.h"

namespace Component
{
    class EllipsoidFit : public Framework::DeviceCali
    {
    public:
        explicit EllipsoidFit(float unit);

        void cali(Common::Vec3 *data, uint16_t num, Framework::DeviceCaliData &result) override;

    private:
        float *D = nullptr;
        float *A_tmp = nullptr;
        float _unit = 1.0f;
    };
}

#endif //ELLIPSOID_FIT_H_
