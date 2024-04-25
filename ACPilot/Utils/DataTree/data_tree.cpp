//
// Created by zhaohe on 2024/1/21.
//

#include <string.h>
#include "data_tree.h"
#include "type.h"
#include "error_handing.h"
#include "Url/url.h"
#include "default_debug.h"

bool jsonArrayNodeIsUsed(JsonTree *node)
{
    if (nullptr == node)
    {
        return false;
    }

    if (0 == strncmp(node->getKey(), "0", PARAM_NAME_LEN))
    {
        return false;
    }

    return true;
}

bool dataArrayNodeIsUsed(DataTree *node)
{
    if (nullptr == node)
    {
        return false;
    }

    if (0 == strncmp(node->getKey(), "0", PARAM_NAME_LEN))
    {
        return false;
    }

    return true;
}

void dataArrayNodeSetIndex(DataTree *node, uint16_t index)
{
    snprintf(node->getKey(), PARAM_NAME_LEN, "%u", index);
}

uint16_t dataArrayNodeGetIndex(DataTree *node)
{
    return strtol(node->getKey(), nullptr, 10);
}

void jsonArrayNodeSetIndex(JsonTree *node, uint16_t index)
{
    snprintf(node->getKey(), PARAM_NAME_LEN, "%u", index);
}

uint16_t jsonArrayNodeGetIndex(JsonTree *node)
{
    return strtol(node->getKey(), nullptr, 10);
}

class DataTreeDataFromJson : public TreeVisit
{
public:
    explicit DataTreeDataFromJson(DataTree *node);

    AC_RET operator()(Tree *node, TREE_VISIT_ORDER order) override;

private:
    DataTree *_data_root = nullptr;
    Url _url;
};

DataTreeDataFromJson::DataTreeDataFromJson(DataTree *node)
{
    _data_root = node;
}

AC_RET DataTreeDataFromJson::operator()(Tree *node, TREE_VISIT_ORDER order)
{
    JsonTree *json_node = static_cast<JsonTree *>(node);
    DataTree *data_node = nullptr;

    if (JSON_TYPE_ARRAY == json_node->getType())
    {
        BASE_ERROR("Array is not supported to set");
        return AC_ERROR;
    }
    if (TREE_PRE_ORDER == order)
    {
        _url.push(json_node->getKey());

        if (nullptr == json_node->getFirstChild())
        {
            data_node = DataTree::find(_data_root, _url);
            if (nullptr == data_node)
            {
                return AC_OK;
            }
            data_node->singleNodeFromJson(json_node);
        }
    } else if (TREE_POST_ORDER == order)
    {
        _url.pop();
    }

    return AC_OK;
}

class DataTreeDataToJson : public TreeVisit
{
public:
    AC_RET operator()(Tree *node, TREE_VISIT_ORDER order) override;

    JsonTree *getRes();

private:
    JsonTree *_root = nullptr;
    JsonTree *_current_node = nullptr;
};

AC_RET DataTreeDataToJson::operator()(Tree *node, TREE_VISIT_ORDER order)
{
    JsonTree *json_node = nullptr;
    DataTree *data_node = static_cast<DataTree *>(node);

    if (nullptr == data_node)
    {
        return AC_ERROR;
    }
    /* 后序遍历, 回到当前节点 */
    if (TREE_POST_ORDER == order)
    {
        /* 如果当前节点是数组, 则仅保留已使用节点 */
        if (JSON_TYPE_ARRAY == _current_node->getType())
        {
            JsonTree *child = _current_node->getFirstChild();
            JsonTree *next_child = nullptr;
            NULL_CHECK(child);
            while (nullptr != child)
            {
                next_child = child->getNeighbor();
                if (!jsonArrayNodeIsUsed(child))
                {
                    _current_node->delChild(child);
                    Json::free(child);
                }
                child = next_child;
            }
        }
        /* 回到父节点 */
        _current_node = _current_node->getParent();
        return AC_OK;
    } else if (TREE_PRE_ORDER == order) /* 前序遍历, 建立新节点 */
    {
        /* 为当前节点分配空间 */
        NULL_CHECK(json_node = Json::alloc());

        data_node->singleNodeToJson(json_node);

        if (nullptr == _root)
        {
            _root = json_node;
            _current_node = json_node;
        } else
        {
            _current_node->addChild(json_node);
            _current_node = json_node;
        }
    }
    return AC_OK;
    error:
    _root = nullptr;
    return AC_ERROR;
}

JsonTree *DataTreeDataToJson::getRes()
{
    return _root;
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

AC_RET DataTree::setData(void *data, uint16_t size)
{
    if (nullptr == data || _size != size)
    {
        return AC_ERROR;
    }
    memcpy(_data, data, _size);
    return AC_OK;
}

AC_RET DataTree::setData(void *data)
{
    _data = data;
    return AC_OK;
}

AC_RET DataTree::copyData(DataTree *tree)
{
    if (nullptr == tree || nullptr == tree->getData() || nullptr == _data || tree->getSize() != _size)
    {
        return AC_ERROR;
    }

    memcpy(_data, tree->getData(), _size);
    return AC_OK;
}

void DataTree::clearData()
{
    memset(_data, 0, _size);
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

AC_RET DataTree::singleNodeFromJson(JsonTree *json)
{
    if (nullptr == json)
    {
        return AC_ERROR;
    }
    Type::transStrToData(json->getVal(), _size, _data, _type);
    return AC_OK;
}

AC_RET DataTree::singleNodeToJson(JsonTree *json)
{
    if (nullptr == json)
    {
        return AC_ERROR;
    }
    json->setKey(_key);

    if (_type == AC_ROOT || _type == AC_STRUCT || _type == AC_ARRAY)
    {
        json->setType(static_cast<JSON_TYPE>(_type));
    } else
    {
        json->setType(JSON_TYPE_DATA);
        RETURN_CHECK(Type::transDataToStr(json->getVal(), JSON_VAL_LEN, _data, _type));
    }
    return AC_OK;
    error:
    return AC_ERROR;
}

AC_RET DataTree::singleNodeFromStruct(JsonTree *json)
{
    if (JSON_TYPE_DATA == json->getType())
    {
        _size = Type::transStrToType(json->getVal(), _type);
        if (0 == _size)
        {
            BASE_ERROR("Invalid data type:%s", json->getVal());
            return AC_ERROR;
        }
    } else
    {
        setType(static_cast<AC_DATA_TYPE>(json->getType()));
        setSize(0);
    }
    if (nullptr != json->getParent() && JSON_TYPE_ARRAY == json->getParent()->getType())
    {
        dataArrayNodeSetIndex(this, 0);
    } else
    {
        setKey(json->getKey());
    }

    return AC_OK;
}

AC_RET DataTree::singleNodeToStruct(JsonTree *json)
{
    if (nullptr == json)
    {
        return AC_ERROR;
    }
    json->setKey(_key);

    if (_type == AC_ROOT || _type == AC_STRUCT || _type == AC_ARRAY)
    {
        json->setType(static_cast<JSON_TYPE>(_type));
    } else
    {
        json->setType(JSON_TYPE_DATA);
        RETURN_CHECK(Type::transTypeToStr(json->getVal(), _type));
        if (AC_STRING == _type)
        {
            snprintf(json->getVal(), JSON_VAL_LEN, "%s[%u]", json->getVal(), _size);
        }
    }
    return AC_OK;
    error:
    return AC_ERROR;
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


DataTree *DataTree::find(DataTree *tree, char *url)
{
    if (nullptr == url)
    {
        return nullptr;
    }

    Url url_obj(url);

    return find(tree, url_obj);
}

DataTree *DataTree::find(DataTree *tree, Url &url)
{
    uint16_t index = 0;
    DataTree *ret = tree;

    if (strncmp(tree->getKey(), url.get(index), PARAM_NAME_LEN - 1) != 0)
    {
        return nullptr;
    }
    index++;
    while (nullptr != url.get(index))
    {
        DataTree *child = ret->findChild(url.get(index));
        if (nullptr == child)
        {
            BASE_INFO("Can't find [%s]", url.get(index));
            return nullptr;
        }
        ret = child;

        index++;
    }
    return ret;
}

void DataTree::setFirstChild(DataTree *child)
{
    _setFistChild(child);
}

void DataTree::setNeighbor(DataTree *neighbor)
{
    _setNeighbor(neighbor);
}

void DataTree::setParent(DataTree *child)
{
    _setParent(child);
}

void *DataTree::getData()
{
    return _data;
}

AC_RET DataTree::fromJson(DataTree *tree, JsonTree *json)
{
    if (nullptr == json)
    {
        return AC_ERROR;
    }
    DataTreeDataFromJson visit(tree);

    return json->traverse(visit);
}

JsonTree *DataTree::toJson(DataTree *tree)
{
    DataTreeDataToJson visit;
    tree->traverse(visit);
    return visit.getRes();
}





