//
// Created by zhaohe on 2024/1/21.
//

#ifndef DATA_TREE_H_
#define DATA_TREE_H_

#include "Tree/ac_tree.h"
#include "Json/ac_json.h"

class DataTree : public Tree
{
public:
    AC_RET setKey(char *key);

    char *getKey();

    AC_RET allocData(void *ptr);

    AC_RET setData(void *data, uint16_t size, AC_DATA_TYPE type);

    AC_RET getData(void *data, uint16_t size, AC_DATA_TYPE type);

    uint16_t getSize();

    void setSize(uint16_t size);

    AC_DATA_TYPE getType();

    void setType(AC_DATA_TYPE type);

    AC_RET fromJson(JsonTree *json);

    AC_RET toJson(JsonTree *json);

    DataTree *findChild(char *key);

    DataTree *getFirstChild();

    void setFirstChild(DataTree *child);

    DataTree *getNeighbor();

    void setNeighbor(DataTree *child);

    DataTree *getParent();

    void setParent(DataTree *child);

    void addChild(DataTree *child);

    AC_RET delChild(DataTree *child);

    DataTree *findDataNode(char *key);

    AC_RET traverse(TreeVisit &visit);

private:
    char _key[PARAM_NAME_LEN] = {0};
    AC_DATA_TYPE _type = AC_UNKNOWN;
    uint16_t _size = 0;
    void *_data = nullptr;
};

#endif //DATA_TREE_H_
