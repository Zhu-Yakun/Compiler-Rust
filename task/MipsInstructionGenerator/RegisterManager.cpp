#include "RegisterManager.hpp"

std::string tReg(const Avalue &avalue)
{
    if (avalue.is_param())
    {
        return "$a" + std::to_string(avalue.param);
    }
    else
    {
        return "$t" + std::to_string(avalue.pos);
    }
}

// std::string tReg(const size_t &pos) {
//     return "$t" + std::to_string(pos);
// }

// std::string aReg(const Avalue &avalue) {
//     return "$a" + std::to_string(avalue.param);
// }