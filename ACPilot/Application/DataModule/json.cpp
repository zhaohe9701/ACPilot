#include <string.h>
#include <stdio.h>
#include "type.h"
#include "json.h"
#include "data_module.h"
#include "os.h"

#define STACK_SIZE 10
#define GET_FREE_SPACE(head, tail, max) ((max) - (uint32_t)((tail) - (head)))
class Stack
{
public:
    AC_RET push(char data)
    {
        if (_top >= STACK_SIZE)
        {
            return AC_ERROR;
        }
        _stack[_top++] = data;
        return AC_OK;
    }
    char pop()
    {

        if (0 == _top)
        {
            return 0;
        }
        return _stack[--_top];
    }
    char top()
    {
        if (0 == _top)
        {
            return 0;
        }
        return _stack[_top - 1];
    }
private:
    uint8_t _top = 0;
    char _stack[STACK_SIZE] = {0};
};
static char *gotoNextSymbol(char *json)
{
    if (nullptr == json)
    {
        return nullptr;
    }
    while (' ' == *json || '\t' == *json || '\n' == *json || '\r' == *json)
    {
        json++;
    }
    return json;
}

static char *copyKey(char *json, char *key, uint16_t max_len)
{
    if (nullptr == json || nullptr == key)
    {
        return nullptr;
    }
    char *json_ptr = json;
    char *key_ptr = key;
    if ('"' != *json_ptr)
    {
        printf("json format error:miss '\"'\n");
        return nullptr;
    }
    json_ptr++;

    while ('"' != *json_ptr)
    {
        *key_ptr++ = *json_ptr++;
        if (key_ptr - key >= max_len)
        {
            printf("json format error:key too long\n");
            return nullptr;
        }
    }
    *key_ptr = '\0';
    return json_ptr;
}

static char *pushStr(char *json, const char *str, uint32_t max_len)
{
    uint32_t len = strlen(str);
    if (nullptr == json || nullptr == str || len >= max_len)
    {
        return nullptr;
    }
    strncpy(json, str, max_len - 1);
    return json + len;
}
#if 0
AC_RET Json::toDm(DataNode *dm, char *json)
{
    if (nullptr == dm || nullptr == json)
    {
        return AC_ERROR;
    }
    Stack stack;
    char key[PARAM_NAME_LEN] = {0};
    char *ptr = gotoNextSymbol(json);
    DataNode *node = nullptr;
    if ('{' != *ptr)
    {
        printf("json format error:miss '{'\n");
        return AC_ERROR;
    }
    stack.push('{');
    ptr = gotoNextSymbol(++ptr);
    node = dm;
    while (true)
    {
        // 父节点不是数组
        if (nullptr == node->getParent() || AC_ARRAY != node->getParent()->getType())
        {
            // 非数组成员一定有key
            if ( '\"' != *ptr)
            {
                printf("json format error:miss '\"'\n");
                return AC_ERROR;
            }
            // 获取key值
            ptr = copyKey(ptr, key, PARAM_NAME_LEN - 1);
            if (nullptr == ptr)
            {
                return AC_ERROR;
            }
            // 从当前节点的子节点中查找key
            node = node->find(key);
            if (nullptr == node)
            {
                printf("json format error:invalid key\n");
                return AC_ERROR;
            }
            ptr = gotoNextSymbol(++ptr);
            // key后面必须跟':'
            if (':' != *ptr)
            {
                printf("json format error:miss ':'\n");
                return AC_ERROR;
            }
            ptr = gotoNextSymbol(++ptr);
            // 如果value是结构体
            if ('{' == *ptr)
            {
                if (AC_STRUCT != node->getType())
                {
                    printf("json format error:invalid type\n");
                    return AC_ERROR;
                }
                stack.push('{');
                ptr = gotoNextSymbol(++ptr);
                continue;
            }
            // 如果value是数组
            else if ('[' == *ptr)
            {
                if (AC_ARRAY != node->getType())
                {
                    printf("json format error:invalid type\n");
                    return AC_ERROR;
                }
                stack.push('[');
                ptr = gotoNextSymbol(++ptr);
                node = node->getFirstChild();
                continue;
            }
            // 如果value是值
            else if ('\"' == *ptr)
            {
                ptr = node->setDataFromStr(ptr);
            }
            else
            {
                printf("json format error:invalid value\n");
            }
        }
        // 父节点是数组
        else if (nullptr != node->getParent() && AC_ARRAY == node->getParent()->getType())
        {
            // 如果value是值
            if ('\"' == *ptr)
            {
                ptr = node->setDataFromStr(ptr);
            }
            // 如果value是结构体
            else if ('{' == *ptr)
            {
                if (AC_STRUCT != node->getType())
                {
                    printf("json format error:invalid type\n");
                    return AC_ERROR;
                }
                stack.push('{');
                ptr = gotoNextSymbol(++ptr);
                continue;
            }
            // 如果value是数组
            else if ('[' == *ptr)
            {
                printf("json format error:not support nest array\n");
                return AC_ERROR;
            }
            else
            {
                printf("json format error:invalid array value\n");
                return AC_ERROR;
            }
        }
        else
        {
            printf("json format error:miss '\"'\n");
            return AC_ERROR;
        }
        // 只有value是值会到达这里
        if (nullptr == ptr)
        {
            printf("json format error:trans value failed\n");
        }
        ptr = gotoNextSymbol(++ptr);
        // '}'和']'的回溯
        while (',' != *ptr)
        {
            if (*ptr != stack.pop())
            {
                printf("json format error:illegal symbol (%c)\n", *ptr);
                return AC_ERROR;
            }
            node = node->getParent();
            ptr = gotoNextSymbol(++ptr);
            if (nullptr == ptr)
            {
                if (0 == stack.pop())
                {
                    return AC_OK;
                } else
                {
                    printf("json format error:not complete\n");
                    return AC_ERROR;
                }
            }
        }
        ptr = gotoNextSymbol(++ptr);
        node = node->getNeighbour();
    }
}

AC_RET Json::fromDm(DataNode *dm, char *json, uint32_t max_len)
{
    if (nullptr == dm || nullptr == json)
    {
        return AC_ERROR;
    }
    return AC_OK;
    DataNode *node = dm;
    char *ptr = json;
    ptr = pushStr(ptr, "{", GET_FREE_SPACE(json, ptr, max_len));
    if (nullptr == ptr)
    {
        return AC_ERROR;
    }

    node = node->getFirstChild();

    if (nullptr == node)
    {
        ptr = pushStr(ptr, "}", GET_FREE_SPACE(json, ptr, max_len));
        if (nullptr == ptr)
        {
            return AC_ERROR;
        }
        return AC_OK;
    }
    return AC_OK;
    while (node != dm)
    {
        printf("%s\n", json);
        if (AC_ARRAY != node->getParent()->getType())
        {
            ptr = pushStr(ptr, "\"", GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return AC_ERROR;
            }
//            ptr = pushStr(ptr, node->getName(), GET_FREE_SPACE(json, ptr, max_len));
//            if (nullptr == ptr)
//            {
//                return AC_ERROR;
//            }
            ptr = pushStr(ptr, "\":", GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return AC_ERROR;
            }
        }
        if (AC_STRUCT == node->getType())
        {
//            ptr = pushStr(ptr, "{", GET_FREE_SPACE(json, ptr, max_len));
//            if (nullptr == ptr)
//            {
//                return AC_ERROR;
//            }
            node = node->getFirstChild();
            continue;
        }
        else if (AC_ARRAY == node->getType())
        {
//            ptr = pushStr(ptr, "[", GET_FREE_SPACE(json, ptr, max_len));
//            if (nullptr == ptr)
//            {
//                return AC_ERROR;
//            }
            node = node->getFirstChild();
            continue;
        }
        else
        {
//            ptr = pushStr(ptr, "\"", GET_FREE_SPACE(json, ptr, max_len));
//            if (nullptr == ptr)
//            {
//                return AC_ERROR;
//            }
//            if (AC_OK != node->getDataToStr(ptr, GET_FREE_SPACE(json, ptr, max_len)))
//            {
//                return AC_ERROR;
//            }
//            while (*ptr != '\0')
//            {
//                ptr++;
//            }
//            ptr = pushStr(ptr, "\"", GET_FREE_SPACE(json, ptr, max_len));
//            if (nullptr == ptr)
//            {
//                return AC_ERROR;
//            }
        }
        while (nullptr == node->getNeighbour())
        {
//            if (AC_ARRAY == node->getParent()->getType())
//            {
//                ptr = pushStr(ptr, "]", GET_FREE_SPACE(json, ptr, max_len));
//                if (nullptr == ptr)
//                {
//                    return AC_ERROR;
//                }
//            }
//            else
//            {
//                ptr = pushStr(ptr, "}", GET_FREE_SPACE(json, ptr, max_len));
//                if (nullptr == ptr)
//                {
//                    return AC_ERROR;
//                }
//            }
            if (nullptr == node->getParent())
            {
                break;
            }
            node = node->getParent();
        }
        if (nullptr != node->getNeighbour())
        {
//            ptr = pushStr(ptr, ",", GET_FREE_SPACE(json, ptr, max_len));
//            if (nullptr == ptr)
//            {
//                return AC_ERROR;
//            }
            node = node->getNeighbour();
        }
    }
    return AC_OK;
}

DataNode *Json::createDm(char *json)
{

    Stack stack;
    char key[PARAM_NAME_LEN] = {0};
    char value[PARAM_NAME_LEN] = {0};
    char *ptr = gotoNextSymbol(json);
    uint32_t cb_offset = DataModule::cb_offset;
    DataNode *root = nullptr;
    DataNode *node = nullptr;
    DataNode *tmp_node = nullptr;
    AC_DATA_TYPE type = AC_NULL;
    uint16_t size = 0;

    if (nullptr == json)
    {
        goto error;
    }

    if ('{' != *ptr)
    {
        printf("json format error:miss '{'\n");
        goto error;
    }
    stack.push('}');
    node = DataNode::alloc("/", AC_NULL, 0);
    if (nullptr == node)
    {
        printf("json format error:alloc failed\n");
        goto error;
    }
    ptr = gotoNextSymbol(++ptr);
    root = node;
    while (true)
    {
        if ('\"' == *ptr)
        {
            if (']' == stack.top())
            {
                printf("json format error:wrong format\n");
                return nullptr;
            }
            ptr = copyKey(ptr, key, PARAM_NAME_LEN - 1);
            if (nullptr == ptr)
            {
                return nullptr;
            }
            ptr = gotoNextSymbol(++ptr);
            if (':' != *ptr)
            {
                printf("json format error:miss ':'\n");
                return nullptr;
            }
            ptr = gotoNextSymbol(++ptr);
            if ('{' == *ptr)
            {
                stack.push('}');
                tmp_node = DataNode::alloc(key, AC_STRUCT, 0);
                if (nullptr == tmp_node)
                {
                    printf("json format error:alloc failed\n");
                    goto error;
                }
                node->addChild(tmp_node);
                node = tmp_node;
                ptr = gotoNextSymbol(++ptr);
                continue;
            }
            else if ('[' == *ptr)
            {
                stack.push(']');
                tmp_node = DataNode::alloc(key, AC_ARRAY, 0);
                if (nullptr == tmp_node)
                {
                    printf("json format error:alloc failed\n");
                    goto error;
                }
                node->addChild(tmp_node);
                node = tmp_node;
                ptr = gotoNextSymbol(++ptr);
                continue;
            }
            else if ('\"' == *ptr)
            {
                ptr = copyKey(ptr, value, TYPE_BUF_LEN - 1);
                if (nullptr == ptr)
                {
                    return nullptr;
                }
                size = Type::transStrToType(value, type);
                if (0 == size)
                {
                    printf("json format error:invalid type (%s)\n", value);
                    goto error;
                }
                tmp_node = DataNode::alloc(key, type, size);
                if (nullptr == tmp_node)
                {
                    printf("json format error:alloc failed\n");
                    goto error;
                }
                node->addChild(tmp_node);
                node = tmp_node;
            } else
            {
                printf("json format error:invalid value\n");
                goto error;
            }
        } else
        {
            if (']' != stack.top())
            {
                printf("json format error:wrong format\n");
                goto error;
            }
            if ('{' != *ptr)
            {
                printf("json format error:miss '{'\n");
                goto error;
            }
            stack.push('}');
            tmp_node = DataNode::alloc("", AC_STRUCT, 0);
            if (nullptr == tmp_node)
            {
                printf("json format error:alloc failed\n");
                goto error;
            }
            node->addChild(tmp_node);
            node = tmp_node;
            ptr = gotoNextSymbol(++ptr);
            continue;
        }
        ptr = gotoNextSymbol(++ptr);
        while (',' != *ptr)
        {
            if (*ptr != stack.pop())
            {
                printf("json format error:illegal symbol (%c)\n", *ptr);
                goto error;
            }

            ptr = gotoNextSymbol(++ptr);
            if ('\0' == *ptr)
            {
                if (0 == stack.pop())
                {
                    printf("json format SUCCESS\n");
                    return root;
                } else
                {
                    printf("json format error:not complete\n");
                    goto error;
                }
            }
            char name[PARAM_NAME_LEN] = {0};
            node->getName(name);
            printf("1node:%s\n", name);
            node = node->getParent();
            node->getName(name);
            printf("2node:%s\n", name);
        }
        char name[PARAM_NAME_LEN] = {0};
        node->getName(name);
        printf("3node:%s\n", name);
        node = node->getParent();
        if (nullptr == node)
        {
            printf("json format error:wrong format\n");
            goto error;
        }
        ptr = gotoNextSymbol(++ptr);
    }
error:
    return nullptr;
}

AC_RET Json::printDm(DataNode *dm, char *json, uint32_t max_len)
{
    if (nullptr == dm || nullptr == json)
    {
        return AC_ERROR;
    }
    DataNode *node = dm;
    char *ptr = json;
    ptr = pushStr(ptr, "{", GET_FREE_SPACE(json, ptr, max_len));
    if (nullptr == ptr)
    {
        return AC_ERROR;
    }
    node = node->getFirstChild();
    if (nullptr == node)
    {
        ptr = pushStr(ptr, "}", GET_FREE_SPACE(json, ptr, max_len));
        if (nullptr == ptr)
        {
            return AC_ERROR;
        }
        return AC_OK;
    }
    while (true)
    {
        if (AC_ARRAY != node->getParent()->getType())
        {
            ptr = pushStr(ptr, "\"", GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return AC_ERROR;
            }
            ptr = pushStr(ptr, node->getName(), GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return AC_ERROR;
            }
            ptr = pushStr(ptr, "\":", GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return AC_ERROR;
            }
        }
        if (AC_STRUCT == node->getType())
        {
            ptr = pushStr(ptr, "{", GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return AC_ERROR;
            }
            if (nullptr == node->getFirstChild())
            {
                ptr = pushStr(ptr, "}", GET_FREE_SPACE(json, ptr, max_len));
                if (nullptr == ptr)
                {
                    return AC_ERROR;
                }
            }
            else
            {
                node = node->getFirstChild();
                continue;
            }
        }
        else if (AC_ARRAY == node->getType())
        {
            ptr = pushStr(ptr, "[", GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return AC_ERROR;
            }
            if (nullptr == node->getFirstChild())
            {
                ptr = pushStr(ptr, "]", GET_FREE_SPACE(json, ptr, max_len));
                if (nullptr == ptr)
                {
                    return AC_ERROR;
                }
            }
            else
            {
                node = node->getFirstChild();
                continue;
            }
        }
        else
        {
            char type[TYPE_BUF_LEN] = {0};
            if (AC_OK != Type::transTypeToStr(type, node->getType()))
            {
                return AC_ERROR;
            }
            ptr = pushStr(ptr, "\"", GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return AC_ERROR;
            }
            ptr = pushStr(ptr, type, GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return AC_ERROR;
            }
            ptr = pushStr(ptr, "\"", GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return AC_ERROR;
            }
        }
        while (nullptr == node->getNeighbour())
        {
            if (AC_ARRAY == node->getParent()->getType())
            {
                ptr = pushStr(ptr, "]", GET_FREE_SPACE(json, ptr, max_len));
                if (nullptr == ptr)
                {
                    return AC_ERROR;
                }
            }
            else
            {
                ptr = pushStr(ptr, "}", GET_FREE_SPACE(json, ptr, max_len));
                if (nullptr == ptr)
                {
                    return AC_ERROR;
                }
            }
            if (dm == node->getParent())
            {
                printf("json print SUCCESS\n");
                return AC_OK;
            }
            else
            {
                node = node->getParent();
            }
        }
        if (nullptr != node->getNeighbour())
        {
            ptr = pushStr(ptr, ",", GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return AC_ERROR;
            }
            node = node->getNeighbour();
        }
    }
}
#endif

static char *createArrayRecursive(DataNode *node, char *json);
static char *createStructRecursive(DataNode *node, char *json);

static char *createArrayRecursive(DataNode *node, char *json)
{
    char buf[PARAM_NAME_LEN] = {0};
    char *ptr = json;
    DataNode *tmp_node = nullptr;
    do
    {
        ptr = gotoNextSymbol(++ptr);
        if ('\"' == *ptr)
        {
            printf("json format error:wrong format\n");
            return nullptr;
        }

        if ('{' == *ptr)
        {
            tmp_node = DataModule::alloc(buf, AC_STRUCT, 0);
            if (nullptr == tmp_node)
            {
                printf("json format error:alloc failed\n");
                return nullptr;
            }
            node->addChild(tmp_node);
            if (nullptr == (ptr = createStructRecursive(tmp_node, ptr)))
            {
                return nullptr;
            }
        }
        else
        {
            printf("json format error:miss '{'\n");
            return nullptr;
        }
        ptr = gotoNextSymbol(++ptr);
    }
    while (',' == *ptr);
    if (']' != *ptr)
    {
        printf("json format error:miss ']'\n");
        return nullptr;
    }
    return ptr;
}

static char *createStructRecursive(DataNode *node, char *json)
{
    char buf[PARAM_NAME_LEN] = {0};
    char *ptr = json;
    DataNode *tmp_node = nullptr;
    do
    {
        ptr = gotoNextSymbol(++ptr);
        if ('\"' != *ptr)
        {
            printf("json format error:miss '\"'\n");
            return nullptr;
        }
        ptr = copyKey(ptr, buf, PARAM_NAME_LEN - 1);
        if (nullptr == ptr)
        {
            return nullptr;
        }

        ptr = gotoNextSymbol(++ptr);
        if (':' != *ptr)
        {
            printf("json format error:miss ':'\n");
            return nullptr;
        }
        ptr = gotoNextSymbol(++ptr);
        if ('{' == *ptr)
        {
            tmp_node = DataModule::alloc(buf, AC_STRUCT, 0);
            if (nullptr == tmp_node)
            {
                printf("json format error:alloc failed\n");
                return nullptr;
            }
            node->addChild(tmp_node);
            if (nullptr == (ptr = createStructRecursive(tmp_node, ptr)))
            {
                return nullptr;
            }
        }
        else if ('[' == *ptr)
        {
            tmp_node = DataModule::alloc(buf, AC_ARRAY, 0);
            if (nullptr == tmp_node)
            {
                printf("json format error:alloc failed\n");
                return nullptr;
            }
            node->addChild(tmp_node);
            if (nullptr == (ptr = createArrayRecursive(tmp_node, ptr)))
            {
                return nullptr;
            }
        }
        else if ('\"' == *ptr)
        {
            tmp_node = DataModule::alloc(buf, AC_NULL, 0);
            if (nullptr == tmp_node)
            {
                printf("json format error:alloc failed\n");
                return nullptr;
            }
            ptr = copyKey(ptr, buf, PARAM_NAME_LEN - 1);
            if (nullptr == ptr)
            {
                return nullptr;
            }
            AC_DATA_TYPE type = AC_NULL;
            uint32_t size = Type::transStrToType(buf, type);
            tmp_node->setType(type);
            tmp_node->setSize(size);
            node->addChild(tmp_node);
        }
        else
        {
            printf("json format error:invalid value\n");
            return nullptr;
        }
        ptr = gotoNextSymbol(++ptr);
    }
    while (',' == *ptr);
    if ('}' != *ptr)
    {
        printf("json format error:miss '}'\n");
        return nullptr;
    }
    return ptr;
}

static AC_RET allocDataRecursive(DataNode *node)
{
    if (nullptr == node)
    {
        return AC_OK;
    }
    if (AC_ARRAY == node->getType())
    {
        if (AC_OK != node->allocData(DataModule::alloc(sizeof(uint8_t))))
        {
            return AC_ERROR;
        }
        return AC_OK;
    }
    if (AC_STRUCT == node->getType() || AC_NULL == node->getType())
    {
        return allocDataRecursive(node->getFirstChild());
    }
    if (AC_OK != node->allocData(DataModule::alloc(node->getSize())))
    {
        return AC_ERROR;
    }
    return allocDataRecursive(node->getNeighbour());
}

DataNode *Json::createDm(char *json)
{
    if (nullptr == json)
    {
        return nullptr;
    }
    DataNode *root = DataModule::alloc("/", AC_NULL, 0);
    char *ptr = json;
    ptr = gotoNextSymbol(ptr);
    if ('{' != *ptr)
    {
        printf("json format error:miss '{'\n");
        goto error;
    }
    if (nullptr == (ptr = createStructRecursive(root, ptr)))
    {
        goto error;
    }

    DataModule::syncDataAddr();

    if (AC_OK != allocDataRecursive(root))
    {
        goto error;
    }
    return root;
    error:
    DataModule::reset();
    return nullptr;
}


static char *printRecursive(DataNode *node, char *json, char *ptr, uint32_t max_len)
{
    if (nullptr == node)
    {
        return ptr;
    }
    char buf[PARAM_NAME_LEN] = {0};
    do
    {
        if (AC_ARRAY != node->getParent()->getType())
        {
            ptr = pushStr(ptr, "\"", GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return nullptr;
            }
            ptr = pushStr(ptr, node->getName(), GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return nullptr;
            }
            ptr = pushStr(ptr, "\":", GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return nullptr;
            }
        }
        if (AC_STRUCT == node->getType())
        {
            ptr = pushStr(ptr, "{", GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return nullptr;
            }
            if (nullptr == (ptr = printRecursive(node->getFirstChild(), json, ptr, max_len)))
            {
                return nullptr;
            }
            ptr = pushStr(ptr, "}", GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return nullptr;
            }
        }
        else if (AC_ARRAY == node->getType())
        {
            ptr = pushStr(ptr, "[", GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return nullptr;
            }
            if (nullptr == (ptr = printRecursive(node->getFirstChild(), json, ptr, max_len)))
            {
                return nullptr;
            }
            ptr = pushStr(ptr, "]", GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return nullptr;
            }
        }
        else
        {
            if (AC_OK != Type::transTypeToStr(buf, node->getType()))
            {
                return nullptr;
            }
            ptr = pushStr(ptr, "\"", GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return nullptr;
            }
            ptr = pushStr(ptr, buf, GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return nullptr;
            }
            ptr = pushStr(ptr, "\"", GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return nullptr;
            }
        }
        node = node->getNeighbour();
        if (nullptr != node)
        {
            ptr = pushStr(ptr, ",", GET_FREE_SPACE(json, ptr, max_len));
            if (nullptr == ptr)
            {
                return nullptr;
            }
        }
    } while (nullptr != node);

    return ptr;
}

AC_RET Json::printDm(DataNode *dm, char *json, uint32_t max_len)
{
    if (nullptr == dm || nullptr == json)
    {
        return AC_ERROR;
    }
    char *ptr = json;
    ptr = pushStr(ptr, "{", GET_FREE_SPACE(json, ptr, max_len));
    if (nullptr == ptr)
    {
        return AC_ERROR;
    }
    if (nullptr == (ptr = printRecursive(dm->getFirstChild(), json, ptr, max_len)))
    {
        return AC_ERROR;
    }
    ptr = pushStr(ptr, "}", GET_FREE_SPACE(json, ptr, max_len));
    if (nullptr == ptr)
    {
        return AC_ERROR;
    }
    return AC_OK;
}