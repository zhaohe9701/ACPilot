//
// Created by zhaohe on 2024/1/21.
//

#ifndef DATA_TREE_H_
#define DATA_TREE_H_

#include "Tree/ac_tree.h"
#include "Json/ac_json.h"
#include "Url/url.h"

class DataTree : public Tree
{
public:
    AC_RET setKey(char *key);

    char *getKey();

    AC_RET allocData(void *ptr);

    AC_RET setData(void *data, uint16_t size);

    AC_RET setData(void *data);

    AC_RET copyData(DataTree *tree);

    void *getData();

    void clearData();

    uint16_t getSize();

    void setSize(uint16_t size);

    AC_DATA_TYPE getType();

    void setType(AC_DATA_TYPE type);

    AC_RET singleNodeFromJson(JsonTree *json);

    AC_RET singleNodeToJson(JsonTree *json);

    AC_RET singleNodeFromStruct(JsonTree *json);

    AC_RET singleNodeToStruct(JsonTree *json);

    DataTree *getFirstChild();

    void setFirstChild(DataTree *child);

    DataTree *getNeighbor();

    void setNeighbor(DataTree *neighbor);

    DataTree *getParent();

    void setParent(DataTree *child);

    void addChild(DataTree *child);

    AC_RET delChild(DataTree *child);

    DataTree *findChild(char *key);

    AC_RET traverse(TreeVisit &visit);

    static DataTree *find(DataTree *tree, char *url);

    static DataTree *find(DataTree *tree, Url &url);

    static AC_RET fromJson(DataTree *tree, JsonTree *json);

    static JsonTree *toJson(DataTree *tree);

private:
    char _key[PARAM_NAME_LEN] = {0};
    AC_DATA_TYPE _type = AC_UNKNOWN;
    uint16_t _size = 0;
    void *_data = nullptr;
};


bool dataArrayNodeIsUsed(DataTree *node);

bool jsonArrayNodeIsUsed(JsonTree *node);

void dataArrayNodeSetIndex(DataTree *node, uint16_t index);

void jsonArrayNodeSetIndex(JsonTree *node, uint16_t index);

uint16_t dataArrayNodeGetIndex(DataTree *node);

uint16_t jsonArrayNodeGetIndex(JsonTree *node);

#endif //DATA_TREE_H_
