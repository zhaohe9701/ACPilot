//
// Created by zhaohe on 2023/12/11.
//

#include <string.h>
#include "data_node.h"
#include "data_module.h"

static char *comparePathAndName(const char *path, const char *name)
{
    if (nullptr == path || nullptr == name)
    {
        return nullptr;
    }
    uint16_t path_len = strlen(path);
    uint16_t name_len = strlen(name);
    if (path_len < name_len)
    {
        return nullptr;
    }
    if (0 == strncmp(path, name, name_len))
    {
        return (char *)path + name_len;
    }
    return nullptr;
}



AC_RET DataNode::addChild(DataNode *child)
{
    if (nullptr == child)
    {
        return AC_ERROR;
    }

    DataNode *node = _firstChild;
    if (nullptr == node)
    {
        _firstChild = child;
        child->_parent = this;
        return AC_OK;
    }
    while (nullptr != node->_neighbour)
    {
        node = node->_neighbour;
    }
    node->_neighbour = child;
    child->_parent = this;
    return AC_OK;
}

AC_RET DataNode::getData(void *data, int16_t size)
{
    if (nullptr == data || size != _size)
    {
        return AC_ERROR;
    }
    memcpy(data, _data, _size);
    return AC_OK;
}

void *DataNode::getData()
{
    return _data;
}

AC_RET DataNode::setData(void *data)
{
    if (nullptr == data)
    {
        return AC_ERROR;
    }
    memcpy(_data, data, _size);
    return AC_OK;
}

AC_RET DataNode::getName(char *name)
{
    if (nullptr == name)
    {
        return AC_ERROR;
    }
    strncpy(name, _name, PARAM_NAME_LEN - 1);
    return AC_OK;
}

AC_RET DataNode::setName(const char *name)
{
    if (nullptr == name)
    {
        return AC_ERROR;
    }
    strncpy(_name, name, PARAM_NAME_LEN - 1);
    return AC_OK;
}

uint16_t DataNode::getSize()
{
    return _size;
}

AC_RET DataNode::setSize(uint16_t size)
{
    _size = size;
    return AC_OK;
}

AC_DATA_TYPE DataNode::getType()
{
    return _type;
}

AC_RET DataNode::setType(AC_DATA_TYPE type)
{
    _type = type;
    return AC_OK;
}

DataNode *DataNode::find(const char *path)
{
    if (nullptr == path)
    {
        return nullptr;
    }
    char *end = comparePathAndName(path, _name);
    if (nullptr != end && '\0' == *end)
    {
        return this;
    }
    DataNode *node = _firstChild;
    while (nullptr != node)
    {
        DataNode *result = node->find(++end);
        if (nullptr != result)
        {
            return result;
        }
        node = node->_neighbour;
    }
    return nullptr;
}

char *DataNode::setDataFromStr(const char *data)
{
    return 0;
}

AC_RET DataNode::getDataToStr(char *data, uint16_t max_len)
{
    return AC_OK;
}

DataNode * DataNode::getFirstChild()
{
    return _firstChild;
}

DataNode * DataNode::getNeighbour()
{
    return _neighbour;
}

DataNode * DataNode::getParent()
{
    return _parent;
}

AC_RET DataNode::setFirstChild(DataNode *child)
{
    if (nullptr == child)
    {
        return AC_ERROR;
    }
    _firstChild = child;
    child->_parent = this;
    return AC_OK;
}

AC_RET DataNode::setNeighbour(DataNode *neighbour)
{
    if (nullptr == neighbour)
    {
        return AC_ERROR;
    }
    _neighbour = neighbour;
    return AC_OK;
}

char *DataNode::getName()
{
    return _name;
}

AC_RET DataNode::allocData(void *data)
{
    if (nullptr == data)
    {
        return AC_ERROR;
    }
    _data = data;
    return AC_OK;
}

DataNode *DataNode::findChild(const char *name)
{
    if (nullptr == name)
    {
        return nullptr;
    }
    DataNode *node = _firstChild;
    while (nullptr != node)
    {
        if (0 == strncmp(node->_name, name, PARAM_NAME_LEN - 1))
        {
            return node;
        }
        node = node->_neighbour;
    }
    return nullptr;
}

bool DataNode::isEnable()
{
    if (_type != AC_STRUCT || _parent == nullptr || _parent->_type != AC_ARRAY || _data == nullptr)
    {
        return false;
    }
    if (*(int8_t*)_data == FREE_ENTRY)
    {
        return false;
    }
    else
    {
        return true;
    }
}

DataNode *DataNode::findFreeChild()
{
    if (_type != AC_ARRAY)
    {
        return nullptr;
    }
    DataNode *node = _firstChild;
    while (nullptr != node)
    {
        if (node->isEnable())
        {
            node = node->_neighbour;
        }
        else
        {
            return node;
        }
    }
    return nullptr;
}



