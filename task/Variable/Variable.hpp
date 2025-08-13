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
    bool is_mut;     // 是否是可变变量
    bool is_initial; // 是否初始化
    bool is_reference = false; // 是否是引用类型
    bool is_mutable_ref = false; // 如果是引用，是否是可变引用
    std::string ref_target; // 如果是引用，指向的目标变量名（空字符串表示无）

    int param; // 形参，-1表示不是形参，0-3表示对应的$a寄存器，4以上是在栈中

    explicit Variable(const std::string &name, const VariableType &type, const size_t &deep, bool is_mut = false, bool is_initial = false, int param = -1, const size_t &length = 0, const size_t &offset = 0, bool is_reference = false, bool is_mutable_ref = false, std::string ref_target = "")
        : name(name), type(type), offset(offset), length(length), deep(deep), param(param), is_mut(is_mut), is_initial(is_initial), is_reference(is_reference), is_mutable_ref(is_mutable_ref), ref_target(std::move(ref_target))
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
