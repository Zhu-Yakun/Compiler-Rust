#pragma once

#include <unordered_map>
#include <cstring>
#include <string>
#include <iostream>
#include <any>
#include <utility>
#include <optional>
#include <vector>

#include "Variable.hpp"

class VariableTable
{
    std::vector<Variable> variables;

public:
    void pop_until_deep(size_t deep = 0)
    {
        while (!variables.empty() && variables.back().deep > deep)
        {
            variables.pop_back();
        }
    }

    bool add_item(const std::string &name, VariableType type, const bool is_mut,
                  const bool is_initial, const size_t &deep, int param = -1,
                  bool is_reference = false, bool is_mutable_ref = false,
                  const std::string &ref_target = "")
    {
        size_t offset = 0;
        size_t length = 4;
        size_t index = 0;

        // 遮蔽
        bool found = false;
        // std::cout << "name: " << name << " deep: " << deep << std::endl;
        for (auto it = variables.rbegin(); it != variables.rend(); ++it)
        {
            // std::cout << "it->name: " << it->name << " it->deep: " << it->deep << std::endl;
            if (it->name == name && it->deep == deep)
            {
                if (type == VariableType::UNIT)
                {
                    type = it->type; // 用之前的类型
                }
                found = true;
                variables.erase((++it).base()); // 删除当前作用域中旧的
                break;
            }
        }
        if (!found && type == VariableType::UNIT)
        {
            // 如果没有找到同名变量，且类型为 UNIT，则无法推断类型
            std::cerr << "错误：变量 '" << name << "' 没有可遮蔽的旧定义或无法推断类型。" << std::endl;
            return false;
        }

        // 计算 offset（模拟栈帧偏移）
        for (auto &item : variables)
        {
            if (item.deep <= deep)
            {
                offset = item.offset + item.length;
                index++;
            }
            else
                break;
        }

        Variable new_item(name, type, deep, is_mut, is_initial, param, length, offset, is_reference, is_mutable_ref, ref_target);

        if (index == variables.size())
        {
            variables.emplace_back(new_item);
        }
        else
        {
            variables[index] = new_item;
        }
        return true;
    }

    [[nodiscard]] std::optional<Variable> lookup(const std::string &name) const
    {
        for (auto it = variables.rbegin(); it != variables.rend(); ++it)
        {
            if (it->name == name)
            {
                return *it;
            }
        }
        return std::nullopt;
    }

    // 借用检查：是否存在任何指向 target 的引用
    bool has_any_ref_to(const std::string &target) const
    {
        for (const auto &v : variables)
        {
            if (v.is_reference && v.ref_target == target)
                return true;
        }
        return false;
    }

    // 借用检查：是否存在可变引用指向 target
    bool has_mut_ref_to(const std::string &target) const
    {
        for (const auto &v : variables)
        {
            if (v.is_reference && v.is_mutable_ref && v.ref_target == target)
                return true;
        }
        return false;
    }

    void debug_print() const {
        std::cerr << "[VariableTable] 当前变量表: [";
        for (const auto& v : variables) {
            std::cerr << "{ name: " << v.name
                      << ", type: " << v.type
                      << ", deep: " << v.deep
                      << ", is_mut: " << v.is_mut
                      << ", is_initial: " << v.is_initial
                      << " }, ";
        }
        std::cerr << "]" << std::endl;
    }

private:
};