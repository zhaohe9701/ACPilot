/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-11-09 22:39:09
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 23:13:28
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Imu\icm20602.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include <string.h>
#include "icm20602.h"

using namespace Component;

#define READ 0x80
#define WRITE 0x7f

/*************************************************/
#define XG_OFFS_TC_H        0x04
#define XG_OFFS_TC_L        0x05
#define YG_OFFS_TC_H        0x07
#define YG_OFFS_TC_L        0x08
#define ZG_OFFS_TC_H        0x0A
#define ZG_OFFS_TC_L        0x0B
//加速度自检输出(出产时设置，用于与用户的自检输出值比较）
#define SELF_TEST_X_ACCEL   0x0D
#define SELF_TEST_Y_ACCEL   0x0E
#define SELF_TEST_Z_ACCEL   0x0F
//陀螺仪静态偏移
#define XG_OFFS_USRH        0x13
#define XG_OFFS_USRL        0x14
#define YG_OFFS_USRH        0x15
#define YG_OFFS_USRL        0x16
#define ZG_OFFS_USRH        0x17
#define ZG_OFFS_USRL        0x18

#define SMPLRT_DIV          0x19
#define CONFIG              0x1A
#define GYRO_CONFIG         0x1B
#define ACCEL_CONFIG        0x1C
#define ACCEL_CONFIG2       0x1D
#define LP_MODE_CFG         0x1E

//运动唤醒加速度阈值
#define ACCEL_WOM_X_THR     0x20
#define ACCEL_WOM_Y_THR     0x21
#define ACCEL_WOM_Z_THR     0x22

#define FIFO_EN             0x23
#define FSYNC_INT           0x36
#define INT_PIN_CFG         0x37
#define INT_ENABLE          0x38
#define FIFO_WM_INT_STATUS  0x39
#define INT_STATUS          0x3A

//加速度输出
#define ACCEL_XOUT_H        0x3B
#define ACCEL_XOUT_L        0x3C
#define ACCEL_YOUT_H        0x3D
#define ACCEL_YOUT_L        0x3E
#define ACCEL_ZOUT_H        0x3F
#define ACCEL_ZOUT_L        0x40
//温度输出
#define TEMP_OUT_H          0x41
#define TEMP_OUT_L          0x42
//角速度输出
#define GYRO_XOUT_H         0x43
#define GYRO_XOUT_L         0x44
#define GYRO_YOUT_H         0x45
#define GYRO_YOUT_L         0x46
#define GYRO_ZOUT_H         0x47
#define GYRO_ZOUT_L         0x48
//陀螺仪自检输出
#define SELF_TEST_X_GYRO    0x50
#define SELF_TEST_Y_GYRO    0x51
#define SELF_TEST_Z_GYRO    0x52

#define FIFO_WM_TH1         0x60
#define FIFO_WM_TH2         0x61
#define SIGNAL_PATH_RESET   0x68
#define ACCEL_INTEL_CTRL    0x69
#define USER_CTRL           0x6A
//电源控制
#define PWR_MGMT_1          0x6B
#define PWR_MGMT_2          0x6C

#define I2C_IF              0x70
#define FIFO_COUNTH         0x72
#define FIFO_COUNTL         0x73
#define FIFO_R_W            0x74

#define WHO_AM_I            0x75
//加速度静态偏移
#define XA_OFFSET_H         0x77
#define XA_OFFSET_L         0x78
#define YA_OFFSET_H         0x7A
#define YA_OFFSET_L         0x7B
#define ZA_OFFSET_H         0x7D
#define ZA_OFFSET_L         0x7E

Icm20602::Icm20602(Interface::IO *interface)
{
    _interface = interface;
    strncpy(_name, "ICM20602", sizeof(_name));
    _READ = READ;
    _WRITE = WRITE;
    _ability = (1U << Framework::ACCELEROMETER_DEV) |
               (1U << Framework::THERMOMETER_DEV) |
               (1U << Framework::GYROSCOPE_DEV);
}

AC_RET Icm20602::init()
{
    /*复位芯片*/
    _imuWriteRag(PWR_MGMT_1, 0x80);
    tickSleep(100);
    /*复位信号*/
    _imuWriteRag(SIGNAL_PATH_RESET, 0x03);
    tickSleep(100);
    /*使能芯片*/
    _imuWriteRag(PWR_MGMT_1, 0x01);
    tickSleep(100);
    /*屏蔽I2C*/
    _imuWriteRag(I2C_IF, 0x40);
    tickSleep(100);
    /*使能陀螺仪加速度计*/
    _imuWriteRag(PWR_MGMT_2, 0x00);
    tickSleep(100);
    /*采样频率1000Hz*/
    _imuWriteRag(SMPLRT_DIV, 0x00);
    tickSleep(100);
    /*陀螺仪滤波250Hz*/
    _imuWriteRag(CONFIG, 0x01);
    tickSleep(100);
    /*陀螺仪量程2000DPS*/
    _imuWriteRag(GYRO_CONFIG, 0x18 | 0x00);
    tickSleep(100);
    /*加速度计量程16G*/
    _imuWriteRag(ACCEL_CONFIG, 0x18);
    tickSleep(100);
    /*设置中断低电平脉冲*/
    _imuWriteRag(INT_PIN_CFG, 0x10);
    tickSleep(100);
    /*使能数据就绪中断*/
    _imuWriteRag(INT_ENABLE, 0x01);
    tickSleep(100);
    _imuReadRag(WHO_AM_I, 1, &_id);
    _gyro_sensitivity = 0.061035;
    _acc_sensitivity = 0.48828;
    return AC_OK;
}

uint8_t Icm20602::getId()
{
    return _id;
}

AC_RET Icm20602::updateTemp()
{
    uint8_t buf[2];
    int16_t raw;
    _imuReadRag(TEMP_OUT_H, 2, buf);
    raw = ((int16_t) buf[0] << 8) | buf[1];

    _tmp_data.x = 25.0f + (float) raw / 326.8f;

    return AC_OK;
}

AC_RET Icm20602::updateGyro()
{
    uint8_t buf[6];
    uint16_t gx_raw, gy_raw, gz_raw;

    _imuReadRag(GYRO_XOUT_H, 6, buf);

    gx_raw = ((uint16_t) buf[0] << 8) | buf[1];
    gy_raw = ((uint16_t) buf[2] << 8) | buf[3];
    gz_raw = ((uint16_t) buf[4] << 8) | buf[5];

    _gyro_data.x = (float) ((int16_t) (gx_raw)) * _gyro_sensitivity;
    _gyro_data.y = (float) ((int16_t) (gy_raw)) * _gyro_sensitivity;
    _gyro_data.z = (float) ((int16_t) (gz_raw)) * _gyro_sensitivity;

    return AC_OK;
}

AC_RET Icm20602::updateAcc()
{
    uint8_t buf[6];
    uint16_t ax_raw, ay_raw, az_raw;

    _imuReadRag(ACCEL_XOUT_H, 6, buf);

    ax_raw = ((uint16_t) buf[0] << 8) | buf[1];
    ay_raw = ((uint16_t) buf[2] << 8) | buf[3];
    az_raw = ((uint16_t) buf[4] << 8) | buf[5];

    _acc_data.x = (float) ((int16_t) (ax_raw)) * _acc_sensitivity;
    _acc_data.y = (float) ((int16_t) (ay_raw)) * _acc_sensitivity;
    _acc_data.z = (float) ((int16_t) (az_raw)) * _acc_sensitivity;

    return AC_OK;
}

AC_RET Icm20602::getTemp(Framework::TempData &data)
{
    data = _tmp_data;
    return AC_OK;
}

AC_RET Icm20602::getGyro(Framework::GyroData &data)
{
    data = _gyro_data;
    return AC_OK;
}

AC_RET Icm20602::getAcc(Framework::AccData &data)
{
    data = _acc_data;
    return AC_OK;
}