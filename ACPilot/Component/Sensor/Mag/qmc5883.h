//
// Created by zhaohe on 2024/4/16.
//

#ifndef QML5883_H_
#define QML5883_H_


#include "Device/Physical/Mag/mag.h"

#define QMC5883_ADDR 0x0D

namespace Component
{
    class Qmc5883 : public Framework::Mag
    {
    public:
        explicit Qmc5883(Interface::IO *interface);

        AC_RET init() override;

        AC_RET updateMag() override;

        AC_RET getMag(Framework::MagData &data) override;

    private:
        void _magReadReg(uint8_t address, uint8_t length, uint8_t *buf);

        void _magWriteReg(uint8_t address, uint8_t value);
    };
}
#endif //QML5883_H_
