//
// Created by zhaohe on 2024/1/14.
//

#include <string.h>
#include <new>
#include "ac_json.h"
#include "default_debug.h"
#include "error_handing.h"

class FreeJsonTreeVisit : public TreeVisit
{
public:
    AC_RET operator()(Tree *node, TREE_VISIT_ORDER order) override;
};

class SerializeJsonVisit : public TreeVisit
{
public:
    SerializeJsonVisit(char *json, uint32_t len);

    AC_RET operator()(Tree *node, TREE_VISIT_ORDER order) override;

private:
    char *_buf = nullptr;
    uint32_t _len = 0;
};

AC_RET FreeJsonTreeVisit::operator()(Tree *node, TREE_VISIT_ORDER order)
{
    if (TREE_POST_ORDER == order)
    {
        Json::_pool->free((uint8_t *) node);
    }
    return AC_OK;
}

SerializeJsonVisit::SerializeJsonVisit(char *json, uint32_t len)
{
    _buf = json;
    _len = len;
}

AC_RET SerializeJsonVisit::operator()(Tree *node, TREE_VISIT_ORDER order)
{
    JsonTree *tree = static_cast<JsonTree *>(node);
    int len = 0;

    if (TREE_PRE_ORDER == order)
    {
        if (nullptr != tree->getParent() && JSON_TYPE_ARRAY != tree->getParent()->getType())
        {
            if (0 > (len = snprintf(_buf, _len, R"("%s":)", tree->getKey())))
            {
                BASE_ERROR("snprintf error");
                return AC_ERROR;
            }
            _buf += len;
            _len -= len;
        }
        if (JSON_TYPE_ROOT == tree->getType() || JSON_TYPE_STRUCT == tree->getType())
        {
            if (0 > (len = snprintf(_buf, _len, "{")))
            {
                BASE_ERROR("snprintf error");
                return AC_ERROR;
            }
            _buf += len;
            _len -= len;
        } else if (JSON_TYPE_ARRAY == tree->getType())
        {
            if (0 > (len = snprintf(_buf, _len, "[")))
            {
                BASE_ERROR("snprintf error");
                return AC_ERROR;
            }
            _buf += len;
            _len -= len;
        } else if (JSON_TYPE_DATA == tree->getType())
        {
            if (0 > (len = snprintf(_buf, _len, R"("%s")", tree->getVal())))
            {
                BASE_ERROR("snprintf error");
                return AC_ERROR;
            }
            _buf += len;
            _len -= len;
        } else
        {
            BASE_ERROR("unknown type");
            return AC_ERROR;
        }
    } else if (TREE_POST_ORDER == order)
    {
        if (JSON_TYPE_STRUCT == tree->getType() || JSON_TYPE_ROOT == tree->getType())
        {
            if (0 > (len = snprintf(_buf, _len, "}")))
            {
                BASE_ERROR("snprintf error");
                return AC_ERROR;
            }
            _buf += len;
            _len -= len;
        } else if (JSON_TYPE_ARRAY == tree->getType())
        {
            if (0 > (len = snprintf(_buf, _len, "]")))
            {
                BASE_ERROR("snprintf error");
                return AC_ERROR;
            }
            _buf += len;
            _len -= len;
        } else
        {
            if (JSON_TYPE_DATA != tree->getType())
            {
                BASE_ERROR("unknown type");
                return AC_ERROR;
            }
            _buf += len;
            _len -= len;
        }
        if (nullptr != tree->getNeighbor())
        {
            if (0 > (len = snprintf(_buf, _len, ",")))
            {
                BASE_ERROR("snprintf error");
                return AC_ERROR;
            }
            _buf += len;
            _len -= len;
        }
    }
    return AC_OK;
}

AC_RET JsonTree::setKey(char *key)
{
    if (nullptr == key)
    {
        return AC_ERROR;
    }
    strncpy(_key, key, JSON_KEY_LEN);
    return AC_OK;
}

AC_RET JsonTree::setVal(char *val)
{
    if (nullptr == val)
    {
        return AC_ERROR;
    }
    strncpy(_val, val, JSON_VAL_LEN);
    return AC_OK;
}

char *JsonTree::getKey()
{
    return _key;
}

char *JsonTree::getVal()
{
    return _val;
}

JSON_TYPE JsonTree::getType()
{
    return _type;
}

void JsonTree::setType(JSON_TYPE type)
{
    _type = type;
}

JsonTree *JsonTree::findChild(char *key)
{
    if (nullptr == key)
    {
        return nullptr;
    }
    JsonTree *child = getFirstChild();
    while (nullptr != child)
    {
        if (0 == strcmp(child->getKey(), key))
        {
            return child;
        }
        child = child->getNeighbor();
    }
    return nullptr;
}

JsonTree *JsonTree::getFirstChild()
{
    return (JsonTree *) _getFistChild();
}

JsonTree *JsonTree::getNeighbor()
{
    return (JsonTree *) _getNeighbor();
}

JsonTree *JsonTree::getParent()
{
    return (JsonTree *) _getParent();
}

void JsonTree::addChild(JsonTree *child)
{
    return _addChild((Tree *) child);
}

AC_RET JsonTree::delChild(JsonTree *child)
{
    return _delChild((Tree *) child);
}

AC_RET JsonTree::traverse(TreeVisit &visit)
{
    return _traverse(visit);
}

uint16_t JsonTree::getChildrenNum()
{
    return _getChildrenNum();
}

/************************************************************************/

MemoryPool *Json::_pool = nullptr;

static char *skipSpace(char *buf)
{
    while (' ' == *buf || '\t' == *buf || '\n' == *buf || '\r' == *buf)
    {
        buf++;
    }
    return buf;
}

static char *kvCopy(char *dst, char *src, uint32_t len)
{
    if (nullptr == dst || nullptr == src)
    {
        return nullptr;
    }
    while (('"' != *src) && ('\0' != *src) && (len > 1))
    {
        if ('\\' == *src)
        {
            src++;
            continue;
        }
        *dst = *src;
        dst++;
        src++;
        len--;
    }
    if ('"' != *src)
    {
        return nullptr;
    }
    *dst = '\0';
    return src;
}

static char *deserializeStruct(char *buf, JsonTree *tree);

static char *deserializeArray(char *buf, JsonTree *tree);

static char *deserializeData(char *buf, JsonTree *tree);

static char *deserializeData(char *buf, JsonTree *tree)
{
    tree->setType(JSON_TYPE_DATA);
    NULL_CHECK(buf = kvCopy(tree->getVal(), ++buf, JSON_VAL_LEN))
    return ++buf;
    error:
    return nullptr;
}

static char *deserializeArray(char *buf, JsonTree *tree)
{
    JsonTree *child = nullptr;
    tree->setType(JSON_TYPE_ARRAY);
    do
    {
        NULL_CHECK(buf = skipSpace(++buf))                      /* 进入'['或','后跳过空格 */

        NULL_CHECK(child = Json::alloc())                   /* 分配一个节点 */
        tree->addChild(child);                              /* 添加到树中 */
        child->setKey(tree->getKey());                      /* 对于数组子节点,key值为数组节点的key值 */

        if ('"' == *buf)                                        /* 如果是'"',则是数据 */
        {
            NULL_CHECK(buf = deserializeData(buf, child))   /* 添加数据节点 */
        } else if ('{' == *buf)                                 /* 如果是'{',则是结构 */
        {
            NULL_CHECK(buf = deserializeStruct(buf, child))
        } else if ('[' == *buf)                                 /* 如果是'[',则是数组 */
        {
            NULL_CHECK(buf = deserializeArray(buf, child))
        } else
        {
            BASE_ERROR("format error");
            goto error;
        }
        NULL_CHECK(buf = skipSpace(buf))
    } while (',' == *buf);
    if (']' != *buf)
    {
        BASE_ERROR("format error");
        goto error;
    }
    return ++buf;
    error:
    return nullptr;
}

static char *deserializeStruct(char *buf, JsonTree *tree)
{
    JsonTree *child = nullptr;
    tree->setType(JSON_TYPE_STRUCT);
    do
    {
        NULL_CHECK(buf = skipSpace(++buf))                      /* 进入'{'或','后跳过空格 */
        if ('"' != *buf)                                        /* 必需有key */
        {
            BASE_ERROR("format error");
            goto error;
        }
        NULL_CHECK(child = Json::alloc())
        tree->addChild(child);
        NULL_CHECK(buf = kvCopy(child->getKey(), ++buf, JSON_KEY_LEN))   /* 添加到树中 */
        NULL_CHECK(buf = skipSpace(++buf))

        if (':' != *buf)                                        /* 必需有':' */
        {
            BASE_ERROR("format error");
            goto error;
        }

        NULL_CHECK(buf = skipSpace(++buf))

        if ('"' == *buf)                                        /* 如果是'"',则是数据 */
        {
            NULL_CHECK(buf = deserializeData(buf, child))   /* 处理数据节点 */
        } else if ('{' == *buf)                                 /* 如果是'{',则是结构 */
        {
            NULL_CHECK(buf = deserializeStruct(buf, child))
        } else if ('[' == *buf)                                 /* 如果是'[',则是数组 */
        {
            NULL_CHECK(buf = deserializeArray(buf, child))
        } else
        {
            BASE_ERROR("format error");
            goto error;
        }
        NULL_CHECK(buf = skipSpace(buf))
    } while (',' == *buf);

    if ('}' != *buf)
    {
        BASE_ERROR("format error");
        goto error;
    }

    return ++buf;
    error:
    return nullptr;
}

JsonTree *Json::alloc()
{
    if (nullptr == _pool)
    {
        _pool = MemoryPoolManager::find(JSON_MEMORY_POOL_NAME);
        if (nullptr == _pool)
        {
            BASE_ERROR("can't find json memory pool");
            return nullptr;
        }
    }
    /* placement new */
    return new(_pool->alloc()) JsonTree;
}

void Json::free(JsonTree *tree)
{
    FreeJsonTreeVisit freeJsonTree;
    if (nullptr == tree || nullptr == _pool)
    {
        return;
    }
    tree->traverse(freeJsonTree);
}

JsonTree *Json::deserialize(char *json)
{
    JsonTree *root = nullptr;
    NULL_CHECK(root = Json::alloc())
    NULL_CHECK(json = skipSpace(json))
    if ('{' != *json)
    {
        BASE_ERROR("format error");
        goto error;
    }
    NULL_CHECK(json = deserializeStruct(json, root))
    root->setType(JSON_TYPE_ROOT);
    return root;
    error:
    free(root);
    return nullptr;
}

AC_RET Json::serialize(JsonTree *tree, char *json, uint32_t max_len)
{
    if (nullptr == tree || nullptr == json)
    {
        BASE_ERROR("param error");
        return AC_ERROR;
    }
    SerializeJsonVisit serializeJsonVisit(json, max_len);
    return tree->traverse(serializeJsonVisit);
}


