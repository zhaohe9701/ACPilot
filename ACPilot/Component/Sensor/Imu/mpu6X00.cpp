/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-07-07 23:41:27
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 23:13:09
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Imu\mpu6X00.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */

#include <string.h>
#include "mpu6X00.h"

using namespace Component;

#define SELF_TESTX_REG          0X0D    //自检寄存器X
#define SELF_TESTY_REG          0X0E    //自检寄存器Y
#define SELF_TESTZ_REG          0X0F    //自检寄存器Z
#define SELF_TESTA_REG          0X10    //自检寄存器A
#define SAMPLE_RATE_REG         0X19    //采样频率分频器
#define CFG_REG                 0X1A    //配置寄存器
#define GYRO_CFG_REG            0X1B    //陀螺仪配置寄存器
#define ACCEL_CFG_REG           0X1C    //加速度计配置寄存器
#define MOTION_DET_REG          0X1F    //运动检测阀值设置寄存器
#define FIFO_EN_REG             0X23    // FIFO使能寄存器
#define I2CMST_CTRL_REG         0X24    // IIC主机控制寄存器
#define I2CSLV0_ADDR_REG        0X25    // IIC从机0器件地址寄存器
#define I2CSLV0_REG             0X26    // IIC从机0数据地址寄存器
#define I2CSLV0_CTRL_REG        0X27    // IIC从机0控制寄存器
#define I2CSLV1_ADDR_REG        0X28    // IIC从机1器件地址寄存器
#define I2CSLV1_REG             0X29    // IIC从机1数据地址寄存器
#define I2CSLV1_CTRL_REG        0X2A    // IIC从机1控制寄存器
#define I2CSLV2_ADDR_REG        0X2B    // IIC从机2器件地址寄存器
#define I2CSLV2_REG             0X2C    // IIC从机2数据地址寄存器
#define I2CSLV2_CTRL_REG        0X2D    // IIC从机2控制寄存器
#define I2CSLV3_ADDR_REG        0X2E    // IIC从机3器件地址寄存器
#define I2CSLV3_REG             0X2F    // IIC从机3数据地址寄存器
#define I2CSLV3_CTRL_REG        0X30    // IIC从机3控制寄存器
#define I2CSLV4_ADDR_REG        0X31    // IIC从机4器件地址寄存器
#define I2CSLV4_REG             0X32    // IIC从机4数据地址寄存器
#define I2CSLV4_DO_REG          0X33    // IIC从机4写数据寄存器
#define I2CSLV4_CTRL_REG        0X34    // IIC从机4控制寄存器
#define I2CSLV4_DI_REG          0X35    // IIC从机4读数据寄存器

#define I2CMST_STA_REG          0X36    // IIC主机状态寄存器
#define INTBP_CFG_REG           0X37    //中断/旁路设置寄存器
#define INT_EN_REG              0X38    //中断使能寄存器
#define INT_STA_REG             0X3A    //中断状态寄存器

#define ACCEL_XOUTH_REG         0X3B    //加速度值,X轴高8位寄存器
#define ACCEL_XOUTL_REG         0X3C    //加速度值,X轴低8位寄存器
#define ACCEL_YOUTH_REG         0X3D    //加速度值,Y轴高8位寄存器
#define ACCEL_YOUTL_REG         0X3E    //加速度值,Y轴低8位寄存器
#define ACCEL_ZOUTH_REG         0X3F    //加速度值,Z轴高8位寄存器
#define ACCEL_ZOUTL_REG         0X40    //加速度值,Z轴低8位寄存器

#define TEMP_OUTH_REG           0X41    //温度值高八位寄存器
#define TEMP_OUTL_REG           0X42    //温度值低8位寄存器

#define GYRO_XOUTH_REG          0X43    //陀螺仪值,X轴高8位寄存器
#define GYRO_XOUTL_REG          0X44    //陀螺仪值,X轴低8位寄存器
#define GYRO_YOUTH_REG          0X45    //陀螺仪值,Y轴高8位寄存器
#define GYRO_YOUTL_REG          0X46    //陀螺仪值,Y轴低8位寄存器
#define GYRO_ZOUTH_REG          0X47    //陀螺仪值,Z轴高8位寄存器
#define GYRO_ZOUTL_REG          0X48    //陀螺仪值,Z轴低8位寄存器

#define I2CSLV0_DO_REG          0X63    // IIC从机0数据寄存器
#define I2CSLV1_DO_REG          0X64    // IIC从机1数据寄存器
#define I2CSLV2_DO_REG          0X65    // IIC从机2数据寄存器
#define I2CSLV3_DO_REG          0X66    // IIC从机3数据寄存器

#define I2CMST_DELAY_REG        0X67    // IIC主机延时管理寄存器
#define SIGPATH_RST_REG         0X68    //信号通道复位寄存器
#define MDETECT_CTRL_REG        0X69    //运动检测控制寄存器
#define USER_CTRL_REG           0X6A    //用户控制寄存器
#define PWR_MGMT1_REG           0X6B    //电源管理寄存器1
#define PWR_MGMT2_REG           0X6C    //电源管理寄存器2
#define FIFO_CNTH_REG           0X72    // FIFO计数寄存器高八位
#define FIFO_CNTL_REG           0X73    // FIFO计数寄存器低八位
#define FIFO_RW_REG             0X74    // FIFO读写寄存器
#define DEVICE_ID_REG           0X75    //器件ID寄存器,who am i寄存器

Mpu6X00::Mpu6X00(Interface::IO *interface)
{
    _interface = interface;
    strncpy(_name, "MPU6X00", sizeof(_name));
    _ability = (1U << Framework::ACCELEROMETER_DEV) |
               (1U << Framework::THERMOMETER_DEV) |
               (1U << Framework::GYROSCOPE_DEV);
}

AC_RET Mpu6X00::init()
{
    _imuWriteRag(PWR_MGMT1_REG, 0X80); //复位MPU6000
    tickSleep(100);
    _imuWriteRag(SIGPATH_RST_REG, 0X04 | 0x02 | 0x01);
    tickSleep(100);
    _imuWriteRag(PWR_MGMT1_REG, 0X03); //使能MPU6000
    tickSleep(100);
    _imuWriteRag(USER_CTRL_REG, 0x10); //屏蔽IIC接口
    tickSleep(100);
    _imuWriteRag(PWR_MGMT2_REG, 0X00); //加速度与陀螺仪都工作
    tickSleep(100);
    _imuWriteRag(SAMPLE_RATE_REG, 0x00); //设置采样率1000Hz
    tickSleep(1000);
    _imuWriteRag(CFG_REG, 0x01); //设置低通滤波器
    tickSleep(100);
    _imuWriteRag(GYRO_CFG_REG, 0x18 | 0x00); //陀螺仪传感器量程
    tickSleep(100);
    _imuWriteRag(ACCEL_CFG_REG, 0x18); //加速度传感器量程
    tickSleep(100);
    _imuWriteRag(INT_EN_REG, 0x01); //使能数据就绪中断
    tickSleep(100);
    _imuReadRag(DEVICE_ID_REG, 1, &_id);
    _gyro_sensitivity = 0.061035;
    _acc_sensitivity = 0.48828;
    return AC_OK;
}

uint8_t Mpu6X00::getId()
{
    return _id;
}

AC_RET Mpu6X00::updateTemp()
{
    uint8_t buf[2];
    short raw;

    _imuReadRag(TEMP_OUTH_REG, 2, buf);
    raw = ((int16_t) buf[0] << 8) | buf[1];

    _tmp_data.x = 25.0f + (float) raw / 326.8f;

    return AC_OK;
}

AC_RET Mpu6X00::updateGyro()
{
    uint8_t buf[6];
    uint16_t gx_raw, gy_raw, gz_raw;

    _imuReadRag(GYRO_XOUTH_REG, 6, buf);

    gx_raw = ((uint16_t) buf[0] << 8) | buf[1];
    gy_raw = ((uint16_t) buf[2] << 8) | buf[3];
    gz_raw = ((uint16_t) buf[4] << 8) | buf[5];

    _gyro_data.x = (float) ((int16_t) (gx_raw)) * _gyro_sensitivity;
    _gyro_data.y = (float) ((int16_t) (gy_raw)) * _gyro_sensitivity;
    _gyro_data.z = (float) ((int16_t) (gz_raw)) * _gyro_sensitivity;

    return AC_OK;
}

AC_RET Mpu6X00::updateAcc()
{
    uint8_t buf[6];
    uint16_t ax_raw, ay_raw, az_raw;

    _imuReadRag(ACCEL_XOUTH_REG, 6, buf);

    ax_raw = ((uint16_t) buf[0] << 8) | buf[1];
    ay_raw = ((uint16_t) buf[2] << 8) | buf[3];
    az_raw = ((uint16_t) buf[4] << 8) | buf[5];

    _acc_data.x = (float) ((int16_t) (ax_raw)) * _acc_sensitivity;
    _acc_data.y = (float) ((int16_t) (ay_raw)) * _acc_sensitivity;
    _acc_data.z = (float) ((int16_t) (az_raw)) * _acc_sensitivity;

    return AC_OK;
}

AC_RET Mpu6X00::getTemp(Framework::TempData &data)
{
    data = _tmp_data;
    return AC_OK;
}

AC_RET Mpu6X00::getGyro(Framework::GyroData &data)
{
    data = _gyro_data;
    return AC_OK;
}

AC_RET Mpu6X00::getAcc(Framework::AccData &data)
{
    data = _acc_data;
    return AC_OK;
}