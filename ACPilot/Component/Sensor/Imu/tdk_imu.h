//
// Created by zhaohe on 2024/4/13.
//

#ifndef TDK_IMU_H_
#define TDK_IMU_H_

#include "Device/Physical/Imu/imu.h"

namespace Component
{
    class TdkImu : public Framework::Imu
    {
    protected:
        uint8_t _READ = 0x00;
        uint8_t _WRITE = 0x00;
        uint8_t _BANK0 = 0x00;
        uint8_t _REG_BANK_SEL = 0x00;

        float _acc_sensitivity = 0.0f;
        float _gyro_sensitivity = 0.0f;

        void _imuWriteRag(uint8_t bank, uint8_t address, uint8_t value);

        void _imuReadRag(uint8_t bank, uint8_t address, uint8_t length, uint8_t *buf);

        void _imuWriteRag(uint8_t address, uint8_t value);

        void _imuReadRag(uint8_t address, uint8_t length, uint8_t *buf);
    };
}
#endif //TDK_IMU_H_
