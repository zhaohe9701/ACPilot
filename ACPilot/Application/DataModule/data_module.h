//
// Created by zhaohe on 2023/12/10.
//

#ifndef DATA_MODULE_H_
#define DATA_MODULE_H_

#include "type.h"
#include "data.h"
#include "json.h"

class DataModule
{
public:
    static AC_RET load();
    static AC_RET save();
    static AC_RET init(uint32_t size);
    static uint32_t getFreeSize();
    static void reset();
    static DataNode *alloc(const char *name, AC_DATA_TYPE type, uint16_t size);
    static void *alloc(uint16_t size);
    static DataNode *copyWithoutData(DataNode *node);
    static void syncDataAddr();
    template<typename T>
    static AC_RET read(const char *path, T *data);
    template<typename T>
    static AC_RET write(const char *path, T *data);
private:
    static uint8_t *_cb_addr;
    static uint8_t *_data_addr;
    static uint32_t _cb_offset;
    static uint32_t _data_offset;
    static uint32_t _dm_size;
};

#endif //DATA_MODULE_H_
