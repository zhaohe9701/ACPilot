#ifndef JSON_H_
#define JSON_H_

#include "type.h"
#include "data_node.h"
class Json
{
public:
    static AC_RET toDm(DataNode *dm, char *json);
    static AC_RET fromDm(DataNode *dm, char *json, uint32_t max_len);
    static DataNode *createDm(char *json);
    static AC_RET printDm(DataNode *dm, char *json, uint32_t max_len);
};



#endif