#include "VariableType.hpp"
std::string to_string(VariableType type)
{
    switch (type)
    {
    case UNIT:
        return "unit";
    case BOOL:
        return "bool";
    case CHAR:
        return "char";
    case I32:
        return "i32";
    case F32:
        return "f32";
    default:
        return "unknown";
    }
}
