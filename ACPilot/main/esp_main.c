#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "os.h"


extern void boardInit();
extern void deviceInit();
extern void sysInit();
extern void taskInit();

void app_main(void)
{
    tickSleep(10000);

    sysInit();

    boardInit();

//    deviceInit();
//    printf("Hello world2222!\n");
//    sysInit();
//    printf("Hello world3333!\n");
//    taskInit();
//    DataNode node;
    for (;;)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }


}
