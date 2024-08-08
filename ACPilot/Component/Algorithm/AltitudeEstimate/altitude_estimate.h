//
// Created by zhaohe on 2024/5/9.
//

#ifndef ALTITUDE_ESTIMATE_H_
#define ALTITUDE_ESTIMATE_H_


#include "Type/type.h"

namespace Component
{
    class AltitudeEstimate
    {
    public:
        AC_RET init(float r00, float r11, float q00, float q11, float q22);

        AC_RET calculate(float altitude, float acc, float X[3], float dt);

        void clear();

    private:
        float R[4] = {0.f};
        float Q[9] = {0.f};
        float F[9] = {0.f};
        float H[9] = {0.f};
        float P[9] = {0.f};
        float X0[3] = {0.f};
    };
}
#endif //ALTITUDE_ESTIMATE_H_
