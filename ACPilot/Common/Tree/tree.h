//
// Created by zhaohe on 2024/1/13.
//

#ifndef TREE_H_
#define TREE_H_

#include "Type/type.h"

namespace Common
{
    class TreeVisit;

    enum TREE_VISIT_ORDER
    {
        TREE_PRE_ORDER,
        TREE_POST_ORDER,
    };

    class Tree
    {
    protected:
        Tree *_getFistChild();

        void _setFistChild(Tree *child);

        Tree *_getParent();

        void _setParent(Tree *parent);

        Tree *_getNeighbor();

        void _setNeighbor(Tree *neighbor);

        void _addChild(Tree *child);

        AC_RET _delChild(Tree *child);

        uint16_t _getChildrenNum();

        AC_RET _traverse(TreeVisit &visit);

    private:
        Tree *_first_child = nullptr;
        Tree *_parent = nullptr;
        Tree *_neighbor = nullptr;
    };

    class TreeVisit
    {
    public:
        virtual AC_RET operator()(Tree *node, TREE_VISIT_ORDER order) = 0;
    };
}
#endif //TREE_H_
