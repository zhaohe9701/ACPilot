/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-14 23:14:39
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 23:13:18
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Imu\icm42688.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include <string.h>
#include "icm42688.h"

using namespace Component;

#define READ 0x80
#define WRITE 0x7f

/**********REGISTER MAP*************/
#define REG_BANK_SEL                    0x76
#define BANK0                           0
#define BANK1                           1
#define BANK2                           2
#define BANK3                           3
#define BANK4                           4

// User Bank 0
#define DEVICE_CONFIG             0x11
#define DRIVE_CONFIG              0x13
#define INT_CONFIG                0x14
#define FIFO_CONFIG               0x16
#define TEMP_DATA1                0x1D
#define TEMP_DATA0                0x1E
#define ACCEL_DATA_X1             0x1F
#define ACCEL_DATA_X0             0x20
#define ACCEL_DATA_Y1             0x21
#define ACCEL_DATA_Y0             0x22
#define ACCEL_DATA_Z1             0x23
#define ACCEL_DATA_Z0             0x24
#define GYRO_DATA_X1              0x25
#define GYRO_DATA_X0              0x26
#define GYRO_DATA_Y1              0x27
#define GYRO_DATA_Y0              0x28
#define GYRO_DATA_Z1              0x29
#define GYRO_DATA_Z0              0x2A
#define TMST_FSYNCH               0x2B
#define TMST_FSYNCL               0x2C
#define INT_STATUS                0x2D
#define FIFO_COUNTH               0x2E
#define FIFO_COUNTL               0x2F
#define FIFO_DATA                 0x30
#define APEX_DATA0                0x31
#define APEX_DATA1                0x32
#define APEX_DATA2                0x33
#define APEX_DATA3                0x34
#define APEX_DATA4                0x35
#define APEX_DATA5                0x36
#define INT_STATUS2               0x37
#define INT_STATUS3               0x38
#define SIGNAL_PATH_RESET         0x4B
#define INTF_CONFIG0              0x4C
#define INTF_CONFIG1              0x4D
#define PWR_MGMT0                 0x4E
#define GYRO_CONFIG0              0x4F
#define ACCEL_CONFIG0             0x50
#define GYRO_CONFIG1              0x51
#define GYRO_ACCEL_CONFIG0        0x52
#define ACCEL_CONFIG1             0x53
#define TMST_CONFIG               0x54
#define APEX_CONFIG0              0x56
#define SMD_CONFIG                0x57
#define FIFO_CONFIG1              0x5F
#define FIFO_CONFIG2              0x60
#define FIFO_CONFIG3              0x61
#define FSYNC_CONFIG              0x62
#define INT_CONFIG0               0x63
#define INT_CONFIG1               0x64
#define INT_SOURCE0               0x65
#define INT_SOURCE1               0x66
#define INT_SOURCE3               0x68
#define INT_SOURCE4               0x69
#define FIFO_LOST_PKT0            0x6C
#define FIFO_LOST_PKT1            0x6D
#define SELF_TEST_CONFIG          0x70
#define WHO_AM_I                  0x75 // should return 0x47

// User Bank 1
#define SENSOR_CONFIG0            0x03
#define GYRO_CONFIG_STATIC2       0x0B
#define GYRO_CONFIG_STATIC3       0x0C
#define GYRO_CONFIG_STATIC4       0x0D
#define GYRO_CONFIG_STATIC5       0x0E
#define GYRO_CONFIG_STATIC6       0x0F
#define GYRO_CONFIG_STATIC7       0x10
#define GYRO_CONFIG_STATIC8       0x11
#define GYRO_CONFIG_STATIC9       0x12
#define GYRO_CONFIG_STATIC10      0x13
#define XG_ST_DATA                0x5F
#define YG_ST_DATA                0x60
#define ZG_ST_DATA                0x61
#define TMSTAL0                   0x63
#define TMSTAL1                   0x64
#define TMSTAL2                   0x62
#define INTF_CONFIG4              0x7A
#define INTF_CONFIG5              0x7B
#define INTF_CONFIG6              0x7C

// User Bank 2
#define ACCEL_CONFIG_STATIC2      0x03
#define ACCEL_CONFIG_STATIC3      0x04
#define ACCEL_CONFIG_STATIC4      0x05
#define XA_ST_DATA                0x3B
#define YA_ST_DATA                0x3C
#define ZA_ST_DATA                0x3D

// User Bank 4
#define APEX_CONFIG1              0x40
#define APEX_CONFIG2              0x41
#define APEX_CONFIG3              0x42
#define APEX_CONFIG4              0x43
#define APEX_CONFIG5              0x44
#define APEX_CONFIG6              0x45
#define APEX_CONFIG7              0x46
#define APEX_CONFIG8              0x47
#define APEX_CONFIG9              0x48
#define ACCEL_WOM_X_THR           0x4A
#define ACCEL_WOM_Y_THR           0x4B
#define ACCEL_WOM_Z_THR           0x4C
#define INT_SOURCE6               0x4D
#define INT_SOURCE7               0x4E
#define INT_SOURCE8               0x4F
#define INT_SOURCE9               0x50
#define INT_SOURCE10              0x51
#define OFFSET_USER0              0x77
#define OFFSET_USER1              0x78
#define OFFSET_USER2              0x79
#define OFFSET_USER3              0x7A
#define OFFSET_USER4              0x7B
#define OFFSET_USER5              0x7C
#define OFFSET_USER6              0x7D
#define OFFSET_USER7              0x7E
#define OFFSET_USER8              0x7F

/***********************************/

Icm42688::Icm42688(Interface::IO *interface)
{
    _interface = interface;
    strncpy(_name, "ICM42688", sizeof(_name));
    _READ = READ;
    _WRITE = WRITE;
    _REG_BANK_SEL = REG_BANK_SEL;
    _BANK0 = BANK0;
    _ability = (1U << Framework::ACCELEROMETER_DEV) |
               (1U << Framework::THERMOMETER_DEV) |
               (1U << Framework::GYROSCOPE_DEV);
}

AC_RET Icm42688::init()
{
    _imuReadRag(BANK0, WHO_AM_I, 1, &_id);
    if (_id != 0x47)
    {
        BASE_ERROR("ICM42688: ID error");
        return AC_ERROR;
    }

    /* 复位芯片 */
    _imuWriteRag(DEVICE_CONFIG, 0x01);
    tickSleep(15);
    /* 关闭陀螺仪加速度计 */
    _imuWriteRag(PWR_MGMT0, 0x00);
    tickSleep(15);

    /* 抗混叠滤波器参数        Section 5.3                     */
    /* frequency    delt    deltSqr     bitshift            */
    /* 258HZ        6       36          10                  */
    /* 536HZ        12      144         8                   */
    /* 997HZ        21      440         6                   */
    /* 1962HZ       37      1376        4                   */

    /* 按258Hz配置 */
    _imuWriteRag(REG_BANK_SEL, BANK1);
    tickSleep(1);
    _imuWriteRag(GYRO_CONFIG_STATIC3, 6);                       // delt
    tickSleep(1);
    _imuWriteRag(GYRO_CONFIG_STATIC4, 36 & 0xFF);               // deltSqr
    tickSleep(1);
    _imuWriteRag(GYRO_CONFIG_STATIC5, (36 >> 8) | 10 << 4);     // bitshift
    tickSleep(1);

    _imuWriteRag(REG_BANK_SEL, BANK2);
    tickSleep(1);
    _imuWriteRag(ACCEL_CONFIG_STATIC2, 6);
    tickSleep(1);
    _imuWriteRag(ACCEL_CONFIG_STATIC3, 36 & 0xFF);
    tickSleep(1);
    _imuWriteRag(ACCEL_CONFIG_STATIC4, (36 >> 8) | 10 << 4);
    tickSleep(1);

    /* 配置低通滤波器 */
    _imuWriteRag(REG_BANK_SEL, BANK0);
    tickSleep(1);
    _imuWriteRag(GYRO_ACCEL_CONFIG0, 0xFF); // Low Latency option
    tickSleep(1);

    /* 配置中断 */
    _imuWriteRag(INT_CONFIG, 0x03);     // 设置中断高电平脉冲
    tickSleep(1);
    _imuWriteRag(INT_CONFIG0, 0x00);
    tickSleep(1);
    _imuWriteRag(INT_SOURCE0, 0x08);    // 使能数据就绪中断
    tickSleep(1);
    _imuWriteRag(INT_CONFIG1, 0x60);    // 改变中断脉冲持续时间为8us, 复位中断引脚
    tickSleep(1);

    /* 关闭AFSR */
    uint8_t config_value = 0x0;
    _imuReadRag(INTF_CONFIG1, 1, &config_value);
    tickSleep(1);
    config_value &= ~0xC0;
    config_value |= 0x40;
    _imuWriteRag(INTF_CONFIG1, config_value);
    tickSleep(1);

    /*使能陀螺仪加速度计*/
    _imuWriteRag(PWR_MGMT0, 0x0F);
    tickSleep(1);

    /* 配置量程及输出速率 */
    _imuWriteRag(GYRO_CONFIG0, 0x06);       // 2000dps 1000Hz
    tickSleep(15);
    _imuWriteRag(ACCEL_CONFIG0, 0x06);      // 16g 1000Hz
    tickSleep(15);

    _gyro_sensitivity = 0.061035;
    _acc_sensitivity = 0.48828;
    return AC_OK;
}

uint8_t Icm42688::getId()
{
    return _id;
}

AC_RET Icm42688::updateTemp()
{
    uint8_t buf[2];
    int16_t raw;
    _imuReadRag(TEMP_DATA1, 2, buf);
    raw = ((int16_t) buf[0] << 8) | buf[1];

    _tmp_data.x = 25.0f + (float) raw / 326.8f;

    return AC_OK;
}

AC_RET Icm42688::updateGyro()
{
    uint8_t buf[6];
    uint16_t gx_raw, gy_raw, gz_raw;

    _imuReadRag(GYRO_DATA_X1, 6, buf);

    gx_raw = ((uint16_t) buf[0] << 8) | buf[1];
    gy_raw = ((uint16_t) buf[2] << 8) | buf[3];
    gz_raw = ((uint16_t) buf[4] << 8) | buf[5];

    _gyro_data.x = (float) ((int16_t) (gx_raw)) * _gyro_sensitivity;
    _gyro_data.y = (float) ((int16_t) (gy_raw)) * _gyro_sensitivity;
    _gyro_data.z = (float) ((int16_t) (gz_raw)) * _gyro_sensitivity;

    return AC_OK;
}

AC_RET Icm42688::updateAcc()
{
    uint8_t buf[6];
    uint16_t ax_raw, ay_raw, az_raw;

    _imuReadRag(ACCEL_DATA_X1, 6, buf);

    ax_raw = ((uint16_t) buf[0] << 8) | buf[1];
    ay_raw = ((uint16_t) buf[2] << 8) | buf[3];
    az_raw = ((uint16_t) buf[4] << 8) | buf[5];

    _acc_data.x = (float) ((int16_t) (ax_raw)) * _acc_sensitivity;
    _acc_data.y = (float) ((int16_t) (ay_raw)) * _acc_sensitivity;
    _acc_data.z = (float) ((int16_t) (az_raw)) * _acc_sensitivity;

    return AC_OK;
}

AC_RET Icm42688::getTemp(Framework::TempData &data)
{
    data = _tmp_data;
    return AC_OK;
}

AC_RET Icm42688::getGyro(Framework::GyroData &data)
{
    data = _gyro_data;
    return AC_OK;
}

AC_RET Icm42688::getAcc(Framework::AccData &data)
{
    data = _acc_data;
    return AC_OK;
}