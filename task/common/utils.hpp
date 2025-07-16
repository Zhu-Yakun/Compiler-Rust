#pragma once

#include <string>
namespace UTILS {
    static bool start_with(const std::string& str, const std::string& prefix) {
        if (str.size() < prefix.size()) {
            return false; // 如果主字符串小于前缀字符串，直接返回false
        }
        return str.compare(0, prefix.size(), prefix) == 0; // 比较前缀长度的子串
    }
}