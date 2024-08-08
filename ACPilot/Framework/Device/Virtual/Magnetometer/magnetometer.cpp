//
// Created by zhaohe on 2023/8/30.
//

#include <string.h>
#include "magnetometer.h"

using namespace Framework;
Magnetometer::Magnetometer(const char *name) : VirtualDevice(name)
{
    _type = MAGNETOMETER_DEV;
}

AC_RET Magnetometer::read(MagData &data)
{
    MagData raw_data;
    if (nullptr == _physical_device) goto error;
    RETURN_CHECK(_physical_device->readMagnetometer(raw_data))
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

AC_RET Magnetometer::cali(Common::Vec3 *data, uint16_t num, DeviceCali *cali)
{
    cali->cali(data, num, _cali_data);
    _cali_data.calcTrans();
    return AC_OK;
}

AC_RET Magnetometer::clearCali()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            _cali_data.trans[i][j] = 0;
            _cali_data.rot[i][j] = 0;
        }
    }
    _cali_data.offset.set(0.f, 0.f, 0.f);
    _cali_data.gain.set(1.f, 1.f, 1.f);
    _cali_data.trans[0][0] = 1;
    _cali_data.trans[1][1] = 1;
    _cali_data.trans[2][2] = 1;
    _cali_data.rot[0][0] = 1;
    _cali_data.rot[1][1] = 1;
    _cali_data.rot[2][2] = 1;
    return AC_OK;
}

DeviceCaliData *Magnetometer::getCali()
{
    return &_cali_data;
}

AC_RET Magnetometer::setCali(DeviceCaliData &cali)
{
    memcpy(&_cali_data, &cali, sizeof(DeviceCaliData));
    return AC_OK;
}