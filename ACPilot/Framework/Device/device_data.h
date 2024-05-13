//
// Created by zhaohe on 2024/4/12.
//

#ifndef DEVICE_DATA_H_
#define DEVICE_DATA_H_

#include "type.h"

typedef Vec3 AccData;               /* 加速度数据 */

typedef Vec3 GyroData;              /* 陀螺仪数据 */

typedef Vec3 MagData;               /* 磁力计数据 */

typedef Vec1 TempData;              /* 温度计数据 */

typedef Vec1 AltitudeData;          /* 高度计数据 */

typedef Vec1 AtaData;               /* 气压计数据 */

typedef Vec1 VolData;               /* 电压计数据 */

typedef Vec1 CurData;               /* 电流计数据 */

typedef Vec1 ElectricQuantityData;  /* 电池计数据 */

typedef Vec2 PosData;               /* 定位器数据 */

typedef uint32_t DeviceAbility;


enum VirtualDeviceType
{
    DEFAULT_DEV,
    ACCELEROMETER_DEV,
    ALTIMETER_DEV,
    BAROMETER_DEV,
    GYROSCOPE_DEV,
    LOCATOR_DEV,
    MAGNETOMETER_DEV,
    THERMOMETER_DEV,
    VOLTMETER_DEV,
    AMMETER_DEV,
    COULOMETER_DEV,
};

#endif //DEVICE_DATA_H_
