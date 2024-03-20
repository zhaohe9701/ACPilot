//
// Created by zhaohe on 2024/1/13.
//

#include <stdio.h>
#include "ac_tree.h"
#include "error_handing.h"

Tree *Tree::_getFistChild()
{
    return _first_child;
}

Tree *Tree::_getParent()
{
    return _parent;
}

Tree *Tree::_getNeighbor()
{
    return _neighbor;
}

void Tree::_addChild(Tree *child)
{
    if (nullptr == _first_child)
    {
        _first_child = child;
        child->_parent = this;
        return;
    }
    Tree *node = _first_child;
    while (nullptr != node->_neighbor)
    {
        node = node->_neighbor;
    }
    node->_neighbor = child;
    child->_parent = this;
}

AC_RET Tree::_delChild(Tree *child)
{
    if (nullptr == child)
    {
        return AC_ERROR;
    }
    if (child == _first_child)
    {
        _first_child = child->_neighbor;
        child->_parent = nullptr;
        child->_neighbor = nullptr;
        return AC_OK;
    }
    Tree *node = _first_child;
    while (nullptr != node->_neighbor)
    {
        if (node->_neighbor == child)
        {
            node->_neighbor = child->_neighbor;
            child->_parent = nullptr;
            child->_neighbor = nullptr;
            return AC_OK;
        }
        node = node->_neighbor;
    }
    return AC_ERROR;
}

AC_RET Tree::_traverse(TreeVisit &visit)
{
    Tree *node = _first_child;
    RETURN_CHECK(visit(this, TREE_PRE_ORDER))
    while (nullptr != node)
    {
        RETURN_CHECK(node->_traverse(visit))
        node = node->_neighbor;
    }
    RETURN_CHECK(visit(this, TREE_POST_ORDER))
    return AC_OK;
    error:
    return AC_ERROR;
}

void Tree::_setFistChild(Tree *child)
{
    _first_child = child;
}

void Tree::_setParent(Tree *parent)
{
    _parent = parent;
}

void Tree::_setNeighbor(Tree *neighbor)
{
    _neighbor = neighbor;
}

uint16_t Tree::_getChildrenNum()
{
    uint16_t num = 0;
    Tree *node = _first_child;
    while (nullptr != node)
    {
        num++;
        node = node->_neighbor;
    }
    return num;
}


