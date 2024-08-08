//
// Created by zhaohe on 2024/4/17.
//

#ifndef DEVICE_CALI_H_
#define DEVICE_CALI_H_

#include "Type/type.h"
namespace Framework
{
    class DeviceCaliData
    {
    public:
        Common::Vec3 offset{0, 0, 0};
        Common::Vec3 gain{1, 1, 1};
        float rot[3][3] = {{1, 0, 0},
                           {0, 1, 0},
                           {0, 0, 1}};
        float trans[3][3] = {{1, 0, 0},
                             {0, 1, 0},
                             {0, 0, 1}};

        void calcTrans()
        {
            trans[0][0] = rot[0][0] * gain.x;
            trans[0][1] = rot[0][1] * gain.x;
            trans[0][2] = rot[0][2] * gain.x;
            trans[1][0] = rot[1][0] * gain.y;
            trans[1][1] = rot[1][1] * gain.y;
            trans[1][2] = rot[1][2] * gain.y;
            trans[2][0] = rot[2][0] * gain.z;
            trans[2][1] = rot[2][1] * gain.z;
            trans[2][2] = rot[2][2] * gain.z;
        }
    };

    class DeviceCali
    {
    public:
        virtual void cali(Common::Vec3 *data, uint16_t num, DeviceCaliData &result) = 0;
    };
}
#endif //DEVICE_CALI_H_
