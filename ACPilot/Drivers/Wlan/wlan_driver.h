//
// Created by zhaohe on 2024/4/10.
//

#ifndef WLAN_DRIVER_H_
#define WLAN_DRIVER_H_

#include <esp_wifi_types.h>
#include "Type/type.h"

namespace Driver
{

    enum WlanMode
    {
        WLAN_MODE_STA = 0,
        WLAN_MODE_AP = 1,
    };

    class WlanHandle
    {
    public:
        WlanHandle();

        wifi_config_t config{};
        WlanMode mode = WLAN_MODE_STA;
    };

    class WlanDriver
    {
    public:
        static AC_RET init(WlanHandle *handle);

    private:
        static WlanHandle *_handle;

        static AC_RET _staInit();

        static AC_RET _apInit();

        static void _event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

        static uint16_t _retry_count;
    };
}
#endif //WLAN_DRIVER_H_
