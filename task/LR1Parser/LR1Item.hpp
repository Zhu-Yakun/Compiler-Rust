#pragma once

#include "Production.hpp"
#include "Symbol.hpp"

class LR1Item {
public:
    enum class State {
        SHIFT,   // 移进
        REDUCE,  // 归约
        GOTO,    // 待约
        ACCEPT   // 接受
    };

    LR1Item(Production production, size_t dot_position, Symbol lookahead)
            : production(std::move(production)), dot_position(dot_position), lookahead(std::move(lookahead)) {}

    [[nodiscard]] std::string to_string() const {
        std::string res;
        res += production.lhs.literal + " -> ";
        size_t index = 0;
        for (auto &item: production.rhs) {
            if (index++ == dot_position) res += ".";
            res += item.literal;
        }
        if (index == dot_position) res += ".";
        res += ", " + lookahead.literal;
        return "[" + res + "]";
    }

    [[nodiscard]] Symbol next_symbol() const {
        if (dot_position == production.rhs.size()) return Symbol(SymbolType::Epsilon, "");
        return production.rhs[dot_position];
    }

    [[nodiscard]] Symbol nnext_symbol() const {
        if (dot_position == production.rhs.size()) return Symbol(SymbolType::Epsilon, "");

        LR1Item tmp = *this;
        tmp.dot_position++;
        return tmp.next_symbol();
    }

    Production production;
    size_t dot_position;
    Symbol lookahead;

    friend bool operator==(const LR1Item &lhs, const LR1Item &rhs) {
        return lhs.production == rhs.production &&
               lhs.dot_position == rhs.dot_position &&
               lhs.lookahead == rhs.lookahead;
    }

    friend bool operator<(const LR1Item &lhs, const LR1Item &rhs) {
        if (lhs.production < rhs.production) return true;
        if (rhs.production < lhs.production) return false;
        if (lhs.dot_position < rhs.dot_position) return true;
        if (rhs.dot_position < lhs.dot_position) return false;
        return lhs.lookahead < rhs.lookahead;
    }
};

// 自定义哈希函数
struct LR1ItemHash {
    size_t operator()(const LR1Item &item) const {
        return ProductionHash()(item.production) ^ std::hash<size_t>()(item.dot_position) ^
               SymbolHash()(item.lookahead);
    }
};

// 自定义相等比较函数
struct LR1ItemEqual {
    bool operator()(const LR1Item &lhs, const LR1Item &rhs) const {
        return ProductionEqual()(lhs.production, rhs.production) && lhs.dot_position == rhs.dot_position &&
               SymbolEqual()(lhs.lookahead, rhs.lookahead);
    }
};

