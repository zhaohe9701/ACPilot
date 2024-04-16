//
// Created by zhaohe on 2024/4/10.
//

#include <esp_err.h>
#include <string.h>
#include <nvs_flash.h>
#include "wlan_driver.h"
#include "esp_wifi.h"
#include "default_debug.h"
#include "Notify/notify.h"

#define CONNECT_MAXIMUM_RETRY 3

WlanHandle::WlanHandle()
{
    memset(&config, 0, sizeof(wifi_config_t));
}

uint16_t WlanDriver::_retry_count = 0;
WlanHandle *WlanDriver::_handle = nullptr;

AC_RET WlanDriver::init(WlanHandle *handle)
{
    if (handle == nullptr)
    {
        return AC_ERROR;
    }
    _handle = handle;

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    if (_handle->mode == WLAN_MODE_STA)
    {
        return _staInit();
    } else if (_handle->mode == WLAN_MODE_AP)
    {
        return _apInit();
    }

    return AC_ERROR;
}

AC_RET WlanDriver::_staInit()
{
    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &_event_handler,
                                                        nullptr,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &_event_handler,
                                                        nullptr,
                                                        &instance_got_ip));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &_handle->config));
    ESP_ERROR_CHECK(esp_wifi_start());
    BASE_INFO("wifi start connect to: %s", _handle->config.sta.ssid);
    return AC_OK;
}

AC_RET WlanDriver::_apInit()
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &_event_handler,
                                                        nullptr,
                                                        nullptr));

    if (strlen((char *) _handle->config.ap.password) == 0)
    {
        _handle->config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &_handle->config));
    ESP_ERROR_CHECK(esp_wifi_start());
    BASE_INFO("ap start. ssid:%s password:%s", _handle->config.ap.ssid, _handle->config.ap.password);
    return AC_OK;
}

void WlanDriver::_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (_retry_count < CONNECT_MAXIMUM_RETRY)
        {
            esp_wifi_connect();
            _retry_count++;
            BASE_WARNING("retry to connect to the AP");
            return;
        }

        BASE_ERROR("connect to the AP fail");

    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED)
    {
        BASE_INFO("connect to the AP success");
        Notify::notify(WIFI_CONNECT_SUCCESS_EVENT);
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = static_cast<ip_event_got_ip_t *>(event_data);
        BASE_INFO("got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        BASE_INFO("station connected");
        Notify::notify(WIFI_CONNECT_SUCCESS_EVENT);
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        BASE_INFO("station disconnected");
    }
}


