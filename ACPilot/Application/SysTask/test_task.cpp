//
// Created by zhaohe on 2023/5/20.
//

#include "sys.h"
#include "Spi/spi_driver.h"
#include "Memory/ac_memory.h"
#include "Board/board.h"
void testTask(void *argument)
{
//    Spi *spi = new Spi(Board::spi1_bus, Board::baro_cs_pin);
    Board::baro->init();
    for(;;)
    {
//        uint8_t id[4] = {0};
//        spi->readBytes(0x8D, 1, id);
//        debug_printer->info("ID:%x\n", id[0]);
//        osDelay(1000);
        Board::baro->read();
        BaroData data;
        Board::baro->getAltitude(data);

//        debug_printer->info("Altitude:%.2f\n", data.altitude);
        tickSleep(1000);
    }
}
