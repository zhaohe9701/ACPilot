//
// Created by zhaohe on 2024/4/16.
//

#ifndef MAG_H_
#define MAG_H_

#include "Device/Physical/physical_device.h"
#include "IO/io_interface.h"

class Mag : public PhysicalDevice
{
public:
    Mag() = default;

    virtual AC_RET init();

    virtual AC_RET getMag(MagData &data);

    virtual AC_RET updateMag();

    AC_RET readMagnetometer(MagData &data) override;

protected:
    uint8_t _id = 0x00;
    IoInterface *_interface = nullptr;

    MagData _mag_data;
};

#endif //MAG_H_
