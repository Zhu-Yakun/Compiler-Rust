#pragma once

#include <string>
#include <cstring>
#include <utility>
#include "common/utils.hpp"
#include "common/MetaData.hpp"

static const std::string &TEMP_PREFIX = "#";

// 四元式
class Quater
{
public:
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;
    MetaData meta;

    Quater(std::string op, std::string arg1, std::string arg2, std::string result)
        : op(std::move(op)), arg1(std::move(arg1)), arg2(std::move(arg2)), result(std::move(result)) {}

    [[nodiscard]] std::string to_string() const
    {
        std::string formatted_arg1 = arg1.empty() ? "_" : arg1;
        std::string formatted_arg2 = arg2.empty() ? "_" : arg2;
        return "(" + op + ", " + formatted_arg1 + ", " + formatted_arg2 + ", " + result + ")";
    }

    [[nodiscard]] bool is_jump() const { return op[0] == 'j'; }

    static bool is_number(const std::string &str)
    {
        if (str.empty())
            return false; // 空字符串不是数字

        int start = 0;
        if (str[0] == '-')
        { // 如果是负数，跳过负号
            if (str.size() == 1)
                return false; // 如果字符串只有一个负号，则不是数字
            start = 1;
        }

        for (int i = start; i < str.size(); ++i)
        {
            if (!isdigit(str[i]))
                return false; // 检查每个字符是否为数字
        }

        return true; // 所有检查都通过，是数字
    }

    static bool is_temp(const std::string &str)
    {
        return UTILS::start_with(str, TEMP_PREFIX);
    }
};
