//
// Created by zhaohe on 2023/5/20.
//

#include "Spi/spi_driver.h"
#include "Memory/memory.h"
#include "Board/board_esp32_mini.h"

void testTask(void *argument)
{
    for(;;)
    {
        Board::usb->send((uint8_t *)"hello world\r\n", 13, AC_FOREVER);
        tickSleep(1000);
    }
}
