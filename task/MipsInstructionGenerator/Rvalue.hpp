#pragma once

#include <cstdio>

struct Rvalue {
    std::string variable_id; // 存储的变量ID
    size_t start_point = 0; // 生命周期开始
    size_t end_point = -1;   // 生命周期结束
    bool busy = false;
};