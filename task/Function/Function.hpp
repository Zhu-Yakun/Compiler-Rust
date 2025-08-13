#pragma once

#include <string>
#include <cstring>
#include <utility>
#include <vector>
#include "Variable/Variable.hpp"
#include "MipsInstructionGenerator/CodeList.hpp"

/**
 * 函数
 */
class Function
{
public:
    VariableType return_type;
    std::string name;
    std::vector<Variable> formal_variables;

    Function(const VariableType &return_type, std::string name, std::vector<Variable> formal_variables)
        : return_type(return_type), name(std::move(name)), formal_variables(std::move(formal_variables))
    {
    }
};