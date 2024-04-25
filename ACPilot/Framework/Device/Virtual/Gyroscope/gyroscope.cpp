//
// Created by zhaohe on 2023/8/30.
//

#include <string.h>
#include "gyroscope.h"

Gyroscope::Gyroscope(const char *name) : VirtualDevice(name)
{
    _type = GYROSCOPE_DEV;
}

AC_RET Gyroscope::read(GyroData &data)
{
    GyroData raw_data;
    if (nullptr == _physical_device) goto error;
    RETURN_CHECK(_physical_device->readGyroscope(raw_data));
    data.x = (raw_data.x - _cali_data.offset.x) * _cali_data.trans[0][0] +
             (raw_data.y - _cali_data.offset.y) * _cali_data.trans[1][0] +
             (raw_data.z - _cali_data.offset.z) * _cali_data.trans[2][0];
    data.y = (raw_data.x - _cali_data.offset.x) * _cali_data.trans[0][1] +
             (raw_data.y - _cali_data.offset.y) * _cali_data.trans[1][1] +
             (raw_data.z - _cali_data.offset.z) * _cali_data.trans[2][1];
    data.z = (raw_data.x - _cali_data.offset.x) * _cali_data.trans[0][2] +
             (raw_data.y - _cali_data.offset.y) * _cali_data.trans[1][2] +
             (raw_data.z - _cali_data.offset.z) * _cali_data.trans[2][2];
    return AC_OK;
    error:
    return AC_ERROR;
}

AC_RET Gyroscope::cali(Vec3 *data, uint16_t num, DeviceCali *cali)
{
    cali->cali(data, num, _cali_data);
    BASE_INFO("Gyro offset:\n%f %f %f", _cali_data.offset.x, _cali_data.offset.y, _cali_data.offset.z);
    return AC_OK;
}

AC_RET Gyroscope::clearCali()
{
    _cali_data.offset.set(0.f, 0.f, 0.f);
    return AC_OK;
}

AC_RET Gyroscope::copyRot(DeviceCaliData *cali)
{
    memcpy(_cali_data.rot, cali->rot, sizeof(cali->rot));
    _cali_data.calcTrans();
    BASE_INFO("Gyro trans mat:\n"
              "%f %f %f\n"
              "%f %f %f\n"
              "%f %f %f",
              _cali_data.trans[0][0], _cali_data.trans[0][1], _cali_data.trans[0][2],
              _cali_data.trans[1][0], _cali_data.trans[1][1], _cali_data.trans[1][2],
              _cali_data.trans[2][0], _cali_data.trans[2][1], _cali_data.trans[2][2]);
    return AC_OK;
}

DeviceCaliData *Gyroscope::getCali()
{
    return &_cali_data;
}

AC_RET Gyroscope::setCali(DeviceCaliData &cali)
{
    memcpy(&_cali_data, &cali, sizeof(DeviceCaliData));
    return AC_OK;
}