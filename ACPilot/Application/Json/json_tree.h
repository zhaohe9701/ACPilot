#ifndef JSON_TREE_H_
#define JSON_TREE_H_

#include "type.h"
#include "Command/command_parser.h"

class JsonTree
{
public:

private:
    JsonTree *_first_child = nullptr;
    JsonTree *_neighbor = nullptr;
    JsonTree *findNode(char *url, uint32_t &ptr);
    AC_RET toString(char *buf, uint32_t &ptr, uint32_t len, AC_DATA_TYPE parent_type, CommandMethod command_method);
    AC_RET toBin(uint8_t *bin, uint32_t &ptr, uint32_t len);
    AC_RET fromString(char *buf, uint32_t &ptr);
    AC_RET fromBin(uint8_t *buf);
    void free();
public:
    JsonTree *getFirstChild();
    JsonTree *getNeighbor();
    JsonTree *findChild(char *child_name);
    void addChild(JsonTree *tree);
    void addData(void *in_data, AC_DATA_TYPE in_type, const char *in_name);
    JsonTree *findNode(char *url);
    AC_RET toString(char *buf, uint32_t len);
    AC_RET toCapabilitySet(char *buf, uint32_t len);
    AC_RET fromString(char *buf);
    ~JsonTree();

    char name[PARAM_NAME_LEN] = {0};
    AC_DATA_TYPE type = AC_NULL;
    void *data = nullptr;
};
#endif //JSON_TREE_H_