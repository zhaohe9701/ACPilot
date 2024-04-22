//
// Created by zhaohe on 2023/8/29.
//

#include "accelerometer.h"

Accelerometer::Accelerometer(const char *name) : VirtualDevice(name)
{
    _type = ACCELEROMETER_DEV;
}


AC_RET Accelerometer::read(AccData &data)
{
    AccData raw_data;
    if (nullptr == _physical_device)
    {
        BASE_ERROR("NULL physical_device");
        goto error;
    }
    RETURN_CHECK(_physical_device->readAccelerometer(raw_data))
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

AC_RET Accelerometer::cali(Vec3 *data, uint16_t num, DeviceCali *cali)
{
    cali->cali(data, num, _cali_data);
    _cali_data.calcTrans();
    BASE_INFO("Acc trans mat:\n"
              "%f %f %f\n"
              "%f %f %f\n"
              "%f %f %f",
              _cali_data.trans[0][0], _cali_data.trans[0][1], _cali_data.trans[0][2],
              _cali_data.trans[1][0], _cali_data.trans[1][1], _cali_data.trans[1][2],
              _cali_data.trans[2][0], _cali_data.trans[2][1], _cali_data.trans[2][2]);
    BASE_INFO("Acc offset:\n%f %f %f", _cali_data.offset.x, _cali_data.offset.y, _cali_data.offset.z);
    return AC_OK;
}

AC_RET Accelerometer::clearCali()
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

DeviceCaliData *Accelerometer::getCali()
{
    return &_cali_data;
}
