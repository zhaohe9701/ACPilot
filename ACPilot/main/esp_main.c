#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <nvs_flash.h>
#include <driver/gpio.h>
#include "os.h"


extern void boardInit();
extern void deviceInit();
extern void taskInit();
extern void frameworkInit();
extern void serviceInit();
extern void lightControlInit();

void nvsInit()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
}

void isrInit()
{
    gpio_install_isr_service(0);
}

void app_main(void)
{
    tickSleep(2000);

//    isrInit();

    nvsInit();

    frameworkInit();

    boardInit();

    serviceInit();

    deviceInit();

    lightControlInit();

    taskInit();

    for (;;)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
