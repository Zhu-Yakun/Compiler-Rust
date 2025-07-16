#pragma once

#include "Symbol.hpp"

struct Production {
    Symbol lhs;               // 左手边，非终结符
    std::vector<Symbol> rhs;  // 右手边，非终结符和终结符的 组合

    Production() = default;

    Production(Symbol lhs, std::vector<Symbol> rhs) : lhs(std::move(lhs)), rhs(std::move(rhs)) {}

    [[nodiscard]] std::string to_string() const {
        std::string res;
        res += lhs.literal + " ->";
        for (auto &item: rhs) {
            res += " " + item.literal;
        }
        return "[" + res + "]";
    }

    friend bool operator==(const Production &_lhs, const Production &_rhs) {
        return _lhs.lhs == _rhs.lhs && _lhs.rhs == _rhs.rhs;
    }

    friend bool operator<(const Production &_lhs, const Production &_rhs) {
        if (_lhs.lhs < _rhs.lhs) return true;
        if (_rhs.lhs < _lhs.lhs) return false;
        return _lhs.rhs < _rhs.rhs;
    }

    friend std::ostream &operator<<(std::ostream &os, Production &production) {
        os << production.lhs << " " << production.rhs.size();
        for (auto &symbol: production.rhs) {
            os << " " << symbol;
        }
        return os;
    }

    friend std::istream &operator>>(std::istream &is, Production &production) {
        size_t size;
        is >> production.lhs >> size;
        production.rhs.clear();
        Symbol symbol;
        for (size_t i = 0; i < size; ++i) {
            is >> symbol;
            production.rhs.push_back(symbol);
        }
        return is;
    }
};



struct ProductionHash {
    size_t operator()(const Production &prod) const {
        size_t hash = SymbolHash()(prod.lhs);
        for (const auto &sym: prod.rhs) {
            hash ^= SymbolHash()(sym) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

struct ProductionEqual {
    bool operator()(const Production &lhs, const Production &rhs) const {
        if (!SymbolEqual()(lhs.lhs, rhs.lhs) || lhs.rhs.size() != rhs.rhs.size()) {
            return false;
        }
        for (size_t i = 0; i < lhs.rhs.size(); ++i) {
            if (!SymbolEqual()(lhs.rhs[i], rhs.rhs[i])) {
                return false;
            }
        }
        return true;
    }
};

