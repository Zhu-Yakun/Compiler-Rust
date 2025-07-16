#pragma once
#include <string>

enum VariableType
{
    UNIT,
    BOOL,
    CHAR,
    I32,
    F32,
};

std::string to_string(VariableType type);
