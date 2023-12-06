//
// Created by zhaohe on 2023/8/29.
//

#ifndef DEVICE_H_
#define DEVICE_H_

#define DEVIE_NAME_LEN 16

enum DEVICE_TYPE
{
    DEFAULT_DEV,
    ACCELEROMETER_DEV,
    GYROSCOPE_DEV,
    MAGNETOMETER_DEV,
    ALTIMETER_DEV,
    THERMOMETER_DEV,
    PIEZOMEER_DEV
};
class Device
{
public:
    Device() = default;
    bool isMatch(const char *name);
    ~Device() = default;
protected:
    char _name[DEVIE_NAME_LEN] = {0};
    DEVICE_TYPE _type = DEFAULT_DEV;
};


#endif //DEVICE_H_
