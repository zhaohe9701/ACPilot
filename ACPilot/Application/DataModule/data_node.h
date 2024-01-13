//
// Created by zhaohe on 2023/12/11.
//

#ifndef DATA_NODE_H_
#define DATA_NODE_H_

#include "type.h"

class DataNode
{
    friend class DataModule;
public:
    DataNode() = default;
    AC_RET allocData(void *data);
    AC_RET addChild(DataNode *child);
    AC_RET getData(void *data, int16_t size);
    void *getData();
    AC_RET setData(void *data);
    uint16_t getSize();
    AC_RET setSize(uint16_t size);
    DataNode *find(const char *path);
    DataNode *findChild(const char *name);
    AC_RET getDataToStr(char *data, uint16_t max_len);
    char  *setDataFromStr(const char *data);
    AC_RET getName(char *name);
    char *getName();
    AC_RET setName(const char *name);
    AC_RET setType(AC_DATA_TYPE type);
    AC_DATA_TYPE getType();
    DataNode * getFirstChild();
    AC_RET setFirstChild(DataNode *child);
    DataNode * getNeighbour();
    AC_RET setNeighbour(DataNode *neighbour);
    DataNode * getParent();
    DataNode *findFreeChild();
    bool isEnable();
private:
    char _name[PARAM_NAME_LEN] = {0};
    uint16_t _size = 0;
    AC_DATA_TYPE _type = AC_NULL;
    DataNode *_parent = nullptr;
    DataNode *_neighbour = nullptr;
    DataNode *_firstChild = nullptr;
    void *_data = nullptr;
};

enum
{
    FREE_ENTRY = 1,
    USED_ENTRY = 2
};

#endif //DATA_NODE_H_
