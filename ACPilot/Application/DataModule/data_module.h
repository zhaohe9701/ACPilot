//
// Created by zhaohe on 2024/1/22.
//

#ifndef DATA_MODULE_H_
#define DATA_MODULE_H_


#include "Json/ac_json.h"
#include "data_tree.h"

enum DATA_MODULE_REG
{
    DATA_MODULE_NODE_STATIC,
    DATA_MODULE_DATA_STATIC,
    DATA_MODULE_NODE_DYNAMIC,
    DATA_MODULE_DATA_DYNAMIC,
};

class DataModule
{
public:
    static AC_RET init(uint32_t size);

    static AC_RET deInit();

    static void clear();

    static AC_RET load();

    static AC_RET save();

    static AC_RET create(JsonTree *data);

    static AC_RET dump(char *buf, uint32_t len);

    static AC_RET set(char *url, JsonTree *data);

    static AC_RET get(char *url, JsonTree *data);

    static AC_RET add(char *url, JsonTree *data);

    static AC_RET del(char *url);

    static AC_RET read(char *url, void *data, uint16_t size, AC_DATA_TYPE type);

    static AC_RET write(char *url, void *data, uint16_t size, AC_DATA_TYPE type);

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
    static DataTree *_findNode(char *url);

};


#endif //DATA_MODULE_H_
