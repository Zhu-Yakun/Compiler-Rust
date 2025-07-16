#pragma once

#include <string>
#include <cstring>
#include <iostream>
#include <any>
#include "common/MetaData.hpp"

enum class SymbolType
{
    Terminal,    // 终结符
    NonTerminal, // 非终结符
    Epsilon      // 空串
};

class Symbol
{
public:
    SymbolType type;        // 终结符/非终结符/空串
    std::string literal;    // 这个符号的字面量，比如一个非终结符名为 S,A,B e.g.一个终结符名为T_INT,T_xxxx
    std::string real_value; // 这个符号的真实值，比如一个变量名a，一个int数值3
    MetaData meta;          // 元数据，存储一些额外的信息，比如类型、值等

    explicit Symbol(const SymbolType &type = SymbolType::Epsilon, std::string literal = "", std::string real_value = "")
        : type(type), literal(std::move(literal)), real_value(std::move(real_value)), meta() {}

    [[nodiscard]] std::string to_string() const
    {
        return literal; // 或者任何合适的表示方式
    }

    friend bool operator==(const Symbol &lhs, const Symbol &rhs)
    {
        return lhs.type == rhs.type && lhs.literal == rhs.literal;
        // && lhs.real_value == rhs.real_value;
    }

    friend bool operator<(const Symbol &lhs, const Symbol &rhs)
    {
        if (lhs.type != rhs.type)
        {
            return lhs.type < rhs.type;
        }
        // if (lhs.literal != rhs.literal) {
        return lhs.literal < rhs.literal;
        // }
        // return lhs.real_value < rhs.real_value;
    }

    friend std::ostream &operator<<(std::ostream &os, Symbol &symbol)
    {
        if (symbol.type == SymbolType::Epsilon)
        {
            return os << static_cast<int>(symbol.type);
        }
        return os << static_cast<int>(symbol.type) << " " << symbol.literal << " "
                  << (symbol.real_value.empty() ? "NULL" : symbol.real_value);
    }

    friend std::istream &operator>>(std::istream &is, Symbol &symbol)
    {
        int temp;
        is >> temp;
        symbol.type = static_cast<SymbolType>(temp);
        if (symbol.type == SymbolType::Epsilon)
        {
            return is;
        }

        is >> symbol.literal >> symbol.real_value;
        if (symbol.real_value == "NULL")
            symbol.real_value = "";
        return is;
    }
};

struct SymbolHash
{
    size_t operator()(const Symbol &sym) const
    {
        size_t h1 = std::hash<std::string>()(sym.literal);
        // size_t h2 = std::hash<std::string>()(sym.real_value);
        size_t h3 = std::hash<int>()(static_cast<int>(sym.type));

        return h1 ^ h3; // Combine and mix the hashes
    }
};

struct SymbolEqual
{
    bool operator()(const Symbol &lhs, const Symbol &rhs) const
    {
        return lhs.type == rhs.type &&
               lhs.literal == rhs.literal;
        //    && lhs.real_value == rhs.real_value;
    }
};
