//
// Created by zhaohe on 2024/4/16.
//

#include <cstring>
#include "qmc5883.h"
#include "os.h"
#include "Debug/default_debug.h"

using namespace Component;

#define QMC5883L_DATA_X_LSB			0x00		//X LSB			只读
#define QMC5883L_DATA_X_MSB			0x01		//X MSB			只读
#define QMC5883L_DATA_Y_LSB			0x02		//Y LSB			只读
#define QMC5883L_DATA_Y_MSB			0x03		//Y MSB			只读
#define QMC5883L_DATA_Z_LSB			0x04		//Z LSB			只读
#define QMC5883L_DATA_Z_MSB			0x05		//Z MSB			只读
#define QMC5883L_STATUS_REG			0x06		//状态寄存器	只读
#define QMC5883L_TEMP_LSB			0x07		//温度 LSB		只读-温度只是相对值，没有实际用途
#define QMC5883L_TEMP_MSB			0x08		//温度 MSB		只读-温度只是相对值，没有实际用途
#define QMC5883L_CONTROL1_REG		0x09		//控制寄存器1	读写
#define QMC5883L_CONTROL2_REG		0x0A		//控制寄存器2	读写-BIT7为复位
#define QMC5883L_SET_RESET_PERIOD	0x0B		//时间寄存器，设置为0x01
#define QMC5883L_ID_REG				0x0D		//芯片ID		只读

Qmc5883::Qmc5883(Interface::IO *interface)
{
    _interface = interface;
    strncpy(_name, "Qml5883", sizeof(_name));
}

AC_RET Qmc5883::init()
{
    _magReadReg(QMC5883L_ID_REG, 1, &_id);
    if (0xFF != _id)
    {
        BASE_ERROR("can't find Qmc5883");
        return AC_ERROR;
    }
    tickSleep(10);
    _magWriteReg(QMC5883L_CONTROL2_REG, 0x80);      //  复位
    tickSleep(100);
    _magWriteReg(QMC5883L_SET_RESET_PERIOD, 0x01);  //  时钟复位
    tickSleep(50);
    _magWriteReg(QMC5883L_CONTROL1_REG, 0x15);      //  10Hz: 0x11  50Hz: 0x15  100Hz: 0x19 200Hz: 0x1D
    tickSleep(10);
    return AC_OK;
}

AC_RET Qmc5883::updateMag()
{
    uint8_t buf[6] = {0};
    uint16_t mx_raw, my_raw, mz_raw;
    _magReadReg(QMC5883L_DATA_X_LSB, 6, buf);

    mx_raw = ((uint16_t)buf[1] << 8) | buf[0];
    my_raw = ((uint16_t)buf[3] << 8) | buf[2];
    mz_raw = ((uint16_t)buf[5] << 8) | buf[4];
    _mag_data.x = (float)((int16_t)(mx_raw));
    _mag_data.y = (float)((int16_t)(my_raw));
    _mag_data.z = (float)((int16_t)(mz_raw));

    return AC_OK;
}

AC_RET Qmc5883::getMag(Framework::MagData &data)
{
    data = _mag_data;
    return AC_OK;
}

void Qmc5883::_magReadReg(uint8_t address, uint8_t length, uint8_t *buf)
{
    _interface->readBytes(address, length, buf, IO_DEFAULT_TIMEOUT);
}

void Qmc5883::_magWriteReg(uint8_t address, uint8_t value)
{
    _interface->writeReg(address, value, IO_DEFAULT_TIMEOUT);
}


