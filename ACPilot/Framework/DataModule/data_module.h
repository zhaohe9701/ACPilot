//
// Created by zhaohe on 2024/1/22.
//

#ifndef DATA_MODULE_H_
#define DATA_MODULE_H_

#include "dm_template.h"
#include "Json/ac_json.h"
#include "DataTree/data_tree.h"

#define DATA_MODULE_MEMORY_SIZE 10000
#define DATA_MODULE_NVS_NAMESPACE "dm"
#define DATA_MODULE_NVS_KEY "dm"

class DataModule
{
public:
    static AC_RET init();

    static AC_RET deInit();

    static AC_RET load();

    static AC_RET save();

    static AC_RET create(JsonTree *data);

    static AC_RET create(char *data);

    static AC_RET reset();

    static AC_RET dumpStruct(char *buf, uint32_t len);

    static AC_RET dumpData(char *buf, uint32_t len);

    static AC_RET set(const char *url, JsonTree *data);

    static AC_RET set(const char *url, char *data);

    static AC_RET get(const char *url, JsonTree *data);

    static AC_RET get(const char *url, char *data, uint32_t len);

    static AC_RET add(const char *url, JsonTree *data);

    static AC_RET add(const char *url, char *data);

    static AC_RET del(const char *url);

    static AC_RET read(const char *url, void *data, uint16_t size);

    static AC_RET write(const char *url, void *data, uint16_t size);

    static AC_RET doAction(const char *url);

    static AC_RET registerAction(const char *url, TaskFunction action);

    static AC_RET info(char *buf, uint32_t len);

    static DataTree *allocNode();

    static void *alloc(uint16_t size);

    static DataTree *copyTree(DataTree *src);

private:
    static uint8_t *_head;
    static uint8_t *_node_head;
    static uint8_t *_data_head;
    static uint8_t *_ptr;

    static uint32_t _node_size;
    static uint32_t _data_size;
    static uint32_t _size;
    static DataTree *_root;

    static void _sync();
};

#endif //DATA_MODULE_H_
