#pragma once

#include <string>
#include <set>
#include <utility>
#include <vector>
#include <map>
#include <stack>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include "Quater/Quater.hpp"
#include "Production.hpp"
#include "Symbol.hpp"
#include "LR1Item.hpp"
#include "Action.hpp"

class LR1Parser
{
public:
    LR1Parser(const std::vector<Production> &productions, Symbol start, Symbol end);

    explicit LR1Parser(const std::string &file_path);

    LR1Parser() = default;

    void print_firstSet() const;

    void print_tables() const;

    bool parse(const std::vector<Symbol> &sentence);

    Action get_next_action(const Symbol &currentSymbol);

    void save_tables(const std::string &file_path);

    void load_tables(const std::string &file_path);

private:
    void parse_EBNF_line(const std::string &line);

    static void print_stacks(const std::stack<size_t> &stateStack,
                             const std::stack<Symbol> &symbolStack,
                             const std::vector<Symbol> &inputStack);

    void construct_tables();

    /**
     * @brief 求项目集族的闭包
     *
     * @param lr1ItemSet 待求闭包的项目集族
     * @return std::unordered_set<LR1Item, LR1ItemHash, LR1ItemEqual> 该项目集族的闭包
     */
    void closure(std::unordered_set<LR1Item, LR1ItemHash, LR1ItemEqual> &lr1ItemSet) const;

private:
    /**
     * @brief 获取一个LR1项目当前的状态(移进、归约、接受、待约)
     *
     * @param item LR1项目
     * @return LR1Item::State 状态
     */
    [[nodiscard]] LR1Item::State get_lr1item_state(const LR1Item &item) const;

    /**
     * @brief 获取由某非终结符为产生式左边的所有产生式
     *
     * @param symbol 非终结符
     * @return std::vector<Production> 所有产生式
     */
    [[nodiscard]] std::vector<Production> get_productions_start_by_symbol(const Symbol &symbol) const;

private: // 求FIRST集
    void calculate_firstSets();

    [[nodiscard]] std::unordered_set<Symbol, SymbolHash, SymbolEqual>
    get_first_string(const std::vector<Symbol> &content) const;

private:
    // 产生式集合
    std::vector<Production> productions;
    // 按照左部符号（非终结符）分类的产生式集合, 快速获取某个非终结符对应的所有产生式
    std::unordered_map<Symbol, std::vector<Production>, SymbolHash, SymbolEqual> productionMap;
    // 记录每个符号的first集和follow集合
    std::unordered_map<Symbol, std::unordered_set<Symbol, SymbolHash, SymbolEqual>, SymbolHash, SymbolEqual> firstSet, followSet;

    Symbol start_symbol; // 起始符
    Symbol end_symbol;   // 终止符

    std::vector<std::unordered_set<LR1Item, LR1ItemHash, LR1ItemEqual>> lr1ItemSets; // 项目集族

    std::unordered_set<std::string> terminals; // 终结符集

public:
    std::stack<size_t> stateStack;                        // 状态栈
    std::stack<Symbol> symbolStack;                       // 符号栈
    std::map<std::pair<int, Symbol>, int> gotoTable;      // GOTO表
    std::map<std::pair<int, Symbol>, Action> actionTable; // ACTION表
};
