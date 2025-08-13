#pragma once

#include <cstring>
#include <string>

struct Avalue {
    std::string variable_id;

    size_t offset = -1;  // 相对基址的偏移 load == false
    size_t pos = -1;     // 在那个寄存器里 load == true
    size_t start_point = 0; // 生命周期开始
    size_t end_point = -1;   // 生命周期结束

    std::string variable_name;
    int param = -1;
    bool load = false;      // true: 加载在寄存器中； false: 保存在内存中

    [[nodiscard]] bool is_param() const { return UTILS::start_with(variable_name, "param"); }
};