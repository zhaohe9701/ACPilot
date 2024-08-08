//
// Created by zhaohe on 2024/4/22.
//

#ifndef NVS_DRIVER_H_
#define NVS_DRIVER_H_

#include <nvs_flash.h>
#include "Type/type.h"

namespace Driver
{
    class Nvs
    {
    public:
        AC_RET read(const char *name, void *data, uint32_t len) const;

        AC_RET write(const char *name, void *data, uint32_t len) const;

        AC_RET save() const;

        AC_RET erase(const char *name) const;

        static Nvs *open(const char *name);

        static void close(Nvs *nvs);

        static AC_RET eraseAll(const char *name);

        static AC_RET eraseAll(Nvs *nvs);

        static AC_RET info(char *buf, uint32_t len);

    private:
#ifdef C_ESP32
        nvs_handle_t _handle;
#endif
    };
}

#endif //NVS_DRIVER_H_
