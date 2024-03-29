/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-08 23:04:24
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-10 23:54:16
 * @FilePath: \ZH_FLIGHT\Sys\Common\ac_tree.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef AC_TREE_H_
#define AC_TREE_H_

#include "Type/type.h"

class AcTreeNode
{
friend class AcTree;
private:
    AcTreeNode *_first_child = nullptr;
    AcTreeNode *_neighbor = nullptr;
public:
    AcTreeNode *getFirstChild();
    AcTreeNode *getNeighbor();
    void addData(void *in_data, AC_DATA_TYPE in_type, const char *in_name, uint16_t in_len);
    char name[PARAM_NAME_LEN] = {0};
    AC_DATA_TYPE type = AC_NULL;
    uint16_t len = 0;
    void *data = nullptr;
};

class AcTree
{
public:
    static AcTreeNode *findNode(AcTreeNode *tree, char *uri);
    static void addNode(AcTreeNode *root, AcTreeNode *node);
    static AcTreeNode *findChildByName(AcTreeNode *parent, char *name);
};

#endif