#pragma once

#include <unordered_map>
#include <cstring>
#include <string>
#include <iostream>
#include <any>
#include <utility>

#include "VariableType.hpp"
#include "common/utils.hpp"
#include "Quater/Quater.hpp"

struct Variable
{
    std::string id;    // 变量唯一ID
    std::string name;  // 变量名
    VariableType type; // 变量类型
                       //    std::string value; // 变量初始值 （即将舍弃，

    size_t offset; // 偏移量
    size_t length; // 变量长度

    size_t deep;     // 层级，0表示全局变量
    bool is_mut;     // 是否是常量
    bool is_initial; // 是否初始化

    int param; // 形参，-1表示不是形参，0-3表示对应的$a寄存器，4以上是在栈中

    explicit Variable(const std::string &name, const VariableType &type, const size_t &deep, bool is_mut = false, bool is_initial = false, int param = -1, const size_t &length = 0, const size_t &offset = 0)
        : name(name), type(type), offset(offset), length(length), deep(deep), param(param), is_mut(is_mut), is_initial(is_initial)
    {
        // 临时变量的ID: #1 -> -1
        if (Quater::is_temp(name))
        {
            id = name;
        }
        else
        {
            id = std::to_string(++next_id);
        }
    }
    static size_t next_id;
};
