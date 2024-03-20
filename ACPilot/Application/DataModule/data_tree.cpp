//
// Created by zhaohe on 2024/1/21.
//

#include <string.h>
#include "data_tree.h"
#include "type.h"

class FindDataNodeVisit : public TreeVisit
{
public:
    explicit FindDataNodeVisit(char *key);

    AC_RET operator()(Tree *node, TREE_VISIT_ORDER order) override;

    DataTree *getRes();

private:
    char *_key = nullptr;
    DataTree *_node = nullptr;
};

FindDataNodeVisit::FindDataNodeVisit(char *key)
{
    _key = key;
}

AC_RET FindDataNodeVisit::operator()(Tree *node, TREE_VISIT_ORDER order)
{
    DataTree *data_node = static_cast<DataTree *>(node);
    if (nullptr == data_node || nullptr == _key)
    {
        return AC_ERROR;
    }
    if (TREE_POST_ORDER == order)
    {
        if (0 == strncmp(data_node->getKey(), _key, PARAM_NAME_LEN - 1))
        {
            _node = data_node;
        }
    }
    return AC_OK;
}

DataTree *FindDataNodeVisit::getRes()
{
    return _node;
}

AC_RET DataTree::setKey(char *key)
{
    if (nullptr == key)
    {
        return AC_ERROR;
    }
    strncpy(_key, key, PARAM_NAME_LEN);
    return AC_OK;
}

char *DataTree::getKey()
{
    return _key;
}

AC_RET DataTree::allocData(void *ptr)
{
    if (nullptr == ptr)
    {
        return AC_ERROR;
    }
    _data = ptr;
    return AC_OK;
}

AC_RET DataTree::setData(void *data, uint16_t size, AC_DATA_TYPE type)
{
    if (nullptr == data || _size != size || _type != type)
    {
        return AC_ERROR;
    }
    memcpy(_data, data, _size);
    return AC_OK;
}

AC_RET DataTree::getData(void *data, uint16_t size, AC_DATA_TYPE type)
{
    if (nullptr == data || size != _size || type != _type)
    {
        return AC_ERROR;
    }
    memcpy(data, _data, _size);
    return AC_OK;
}

uint16_t DataTree::getSize()
{
    return _size;
}

void DataTree::setSize(uint16_t size)
{
    _size = size;
}

AC_DATA_TYPE DataTree::getType()
{
    return _type;
}

void DataTree::setType(AC_DATA_TYPE type)
{
    _type = type;
}

AC_RET DataTree::fromJson(JsonTree *json)
{
    if (nullptr == json)
    {
        return AC_ERROR;
    }
    strncpy(_key, json->getKey(), PARAM_NAME_LEN);
    return Type::transStrToData(json->getVal(), _data, _type);
}

AC_RET DataTree::toJson(JsonTree *json)
{
    if (nullptr == json)
    {
        return AC_ERROR;
    }
    json->setKey(_key);
    return Type::transDataToStr(json->getVal(), _data, JSON_VAL_LEN, _type);
}

DataTree *DataTree::findChild(char *key)
{
    if (nullptr == key)
    {
        return nullptr;
    }
    DataTree *child = getFirstChild();
    while (nullptr != child)
    {
        if (0 == strncmp(child->getKey(), key, PARAM_NAME_LEN - 1))
        {
            return child;
        }
        child = child->getNeighbor();
    }
    return nullptr;
}

DataTree *DataTree::getFirstChild()
{
    return (DataTree *) _getFistChild();
}

DataTree *DataTree::getNeighbor()
{
    return (DataTree *) _getNeighbor();
}

DataTree *DataTree::getParent()
{
    return (DataTree *) _getParent();
}

void DataTree::addChild(DataTree *child)
{
    _addChild(child);
}

AC_RET DataTree::delChild(DataTree *child)
{
    return _delChild(child);
}

AC_RET DataTree::traverse(TreeVisit &visit)
{
    return _traverse(visit);
}

DataTree *DataTree::findDataNode(char *key)
{
    if (nullptr == key)
    {
        return nullptr;
    }
    FindDataNodeVisit visit(key);
    traverse(visit);
    return visit.getRes();
}

void DataTree::setFirstChild(DataTree *child)
{
    _setFistChild(child);
}

void DataTree::setNeighbor(DataTree *child)
{
    _setNeighbor(child);
}

void DataTree::setParent(DataTree *child)
{
    _setParent(child);
}
