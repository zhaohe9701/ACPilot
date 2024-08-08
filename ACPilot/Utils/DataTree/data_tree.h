//
// Created by zhaohe on 2024/1/21.
//

#ifndef DATA_TREE_H_
#define DATA_TREE_H_

#include "Tree/tree.h"
#include "Json/json.h"
#include "Url/url.h"

namespace Utils
{

    class DataTree : public Common::Tree
    {
    public:
        AC_RET setKey(const char *key);

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

        DataTree *findChild(const char *key);

        AC_RET traverse(Common::TreeVisit &visit);

        static DataTree *find(DataTree *tree, const char *url);

        static DataTree *find(DataTree *tree, Common::Url &url);

        static AC_RET fromJson(DataTree *tree, JsonTree *json);

        static JsonTree *toJson(DataTree *tree);

        static bool dataArrayNodeIsUsed(DataTree *node);

        static void dataArrayNodeSetIndex(DataTree *node, uint16_t index);

        static uint16_t dataArrayNodeGetIndex(DataTree *node);
    private:
        char _key[PARAM_NAME_LEN] = {0};
        AC_DATA_TYPE _type = AC_UNKNOWN;
        uint16_t _size = 0;
        void *_data = nullptr;
    };
}
#endif //DATA_TREE_H_
