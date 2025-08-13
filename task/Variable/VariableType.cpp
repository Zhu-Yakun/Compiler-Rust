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
    case REF_BOOL:
        return "&bool";
    case REF_CHAR:
        return "&char";
    case REF_I32:
        return "&i32";
    case REF_F32:
        return "&f32";
    case REF_MUT_BOOL:
        return "&mut bool";
    case REF_MUT_CHAR:
        return "&mut char";
    case REF_MUT_I32:
        return "&mut i32";
    case REF_MUT_F32:
        return "&mut f32";
    default:
        return "unknown";
    }
}

bool is_reference_type(VariableType type) {
    return type == REF_BOOL || type == REF_CHAR || type == REF_I32 || type == REF_F32 ||
           type == REF_MUT_BOOL || type == REF_MUT_CHAR || type == REF_MUT_I32 || type == REF_MUT_F32;
}

bool is_mutable_reference_type(VariableType type) {
    return type == REF_MUT_BOOL || type == REF_MUT_CHAR || type == REF_MUT_I32 || type == REF_MUT_F32;
}

VariableType get_base_type(VariableType ref_type) {
    switch (ref_type) {
        case REF_BOOL:
        case REF_MUT_BOOL:
            return BOOL;
        case REF_CHAR:
        case REF_MUT_CHAR:
            return CHAR;
        case REF_I32:
        case REF_MUT_I32:
            return I32;
        case REF_F32:
        case REF_MUT_F32:
            return F32;
        default:
            return ref_type; // 如果不是引用类型，返回原类型
    }
}

VariableType create_reference_type(VariableType base_type, bool is_mutable) {
    switch (base_type) {
        case BOOL:
            return is_mutable ? REF_MUT_BOOL : REF_BOOL;
        case CHAR:
            return is_mutable ? REF_MUT_CHAR : REF_CHAR;
        case I32:
            return is_mutable ? REF_MUT_I32 : REF_I32;
        case F32:
            return is_mutable ? REF_MUT_F32 : REF_F32;
        default:
            return UNIT; // 错误情况
    }
}
