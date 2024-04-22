#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "os.h"


extern void boardInit();
extern void deviceInit();
extern void taskInit();
extern void frameworkInit();
extern void serviceInit();
void app_main(void)
{
    tickSleep(10000);

    frameworkInit();

    boardInit();

    serviceInit();

    deviceInit();

    taskInit();

    for (;;)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
