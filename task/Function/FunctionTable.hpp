#pragma once

#include "Function.hpp"
class FunctionTable
{
private:
    std::vector<Function> tables;

public:
    FunctionTable() = default;

    void add_func(const Function &func)
    {
        tables.emplace_back(func);
    }

    std::optional<Function> lookup(const std::string &name)
    {
        for (auto &item : tables)
        {
            if (item.name == name)
            {
                return item;
            }
        }
        return {};
    }

    void update_return_type(Function &fun, VariableType type)
    {
        fun.return_type = type;
        return;
    }
};