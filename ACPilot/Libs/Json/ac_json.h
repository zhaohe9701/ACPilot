//
// Created by zhaohe on 2024/1/14.
//

#ifndef AC_JSON_H_
#define AC_JSON_H_

#include "Tree/ac_tree.h"
#include "MemoryPool/memory_pool.h"
#include "type.h"

#define JSON_KEY_LEN PARAM_NAME_LEN
#define JSON_VAL_LEN DATA_BUF_LEN

#define JSON_MEMORY_POOL_NAME "json"

enum JSON_TYPE
{
    JSON_TYPE_UNKNOWN = AC_UNKNOWN,
    JSON_TYPE_ROOT = AC_ROOT,
    JSON_TYPE_STRUCT = AC_STRUCT,
    JSON_TYPE_ARRAY = AC_ARRAY,
    JSON_TYPE_DATA = AC_DATA,
};

class JsonTree : public Tree
{
public:
    AC_RET setKey(char *key);

    AC_RET setVal(char *val);

    char *getKey();

    char *getVal();

    JSON_TYPE getType();

    void setType(JSON_TYPE type);

    JsonTree *findChild(char *key);

    JsonTree *getFirstChild();

    JsonTree *getNeighbor();

    JsonTree *getParent();

    void addChild(JsonTree *child);

    AC_RET delChild(JsonTree *child);

    AC_RET traverse(TreeVisit &visit);

private:
    JSON_TYPE _type = JSON_TYPE_UNKNOWN;
    char _key[JSON_KEY_LEN] = {0};
    char _val[JSON_VAL_LEN] = {0};
};

class Json
{
    friend class JsonTree;

    friend class FreeJsonTreeVisit;

public:
    static JsonTree *alloc();

    static void free(JsonTree *tree);

    static JsonTree *deserialize(char *json);

    static AC_RET serialize(JsonTree *tree, char *json, uint32_t max_len);

private:
    static MemoryPool *_pool;
};

#endif //AC_JSON_H_
