#pragma once
#include <string>

enum VariableType
{
    UNIT,
    BOOL,
    CHAR,
    I32,
    F32,
    REF_BOOL,       // 不可变引用类型
    REF_CHAR,
    REF_I32,
    REF_F32,
    REF_MUT_BOOL,   // 可变引用类型
    REF_MUT_CHAR,
    REF_MUT_I32,
    REF_MUT_F32,
};

std::string to_string(VariableType type);

// 判断是否为引用类型
bool is_reference_type(VariableType type);

// 判断是否为可变引用类型
bool is_mutable_reference_type(VariableType type);

// 获取引用的基础类型
VariableType get_base_type(VariableType ref_type);

// 创建引用类型
VariableType create_reference_type(VariableType base_type, bool is_mutable);
