//
// Created by zhaohe on 2024/1/22.
//

#ifndef DATA_MODULE_FRAMEWORK_H_
#define DATA_MODULE_FRAMEWORK_H_


#include "Json/ac_json.h"
#include "DataTree/data_tree.h"

#define DATA_MODULE_MEMORY_SIZE 32768

class DataModuleFramework
{
public:
    static AC_RET init();

    static AC_RET deInit();

    static void clear();

    static AC_RET load();

    static AC_RET save();

    static AC_RET create(JsonTree *data);

    static AC_RET create(char *data);

    static AC_RET reset();

    static AC_RET dumpStruct(char *buf, uint32_t len);

    static AC_RET dumpData(char *buf, uint32_t len);

    static AC_RET set(char *url, JsonTree *data);

    static AC_RET set(char *url, char *data);

    static AC_RET get(char *url, JsonTree *data);

    static AC_RET get(char *url, char *data, uint32_t len);

    static AC_RET add(char *url, JsonTree *data);

    static AC_RET add(char *url, char *data);

    static AC_RET del(char *url);

    static AC_RET read(char *url, void *data, uint16_t size);

    static AC_RET write(char *url, void *data, uint16_t size);

    static AC_RET info(char *buf, uint32_t len);

    static DataTree *allocNode();

    static void *alloc(uint16_t size);

    static DataTree *copyTree(DataTree *src);

private:
    static uint8_t *_node_head;
    static uint8_t *_data_head;
    static uint8_t *_ptr;
    static uint32_t _size;
    static DataTree *_root;

    static void _sync();
};

#endif //DATA_MODULE_FRAMEWORK_H_
