#pragma once

#include "Quater/QuaterList.hpp"
#include <vector>
#include <set>
#include "MipsInstructionGenerator/RegisterManager.hpp"
#include "StackManager.hpp"

class MipsInstructionGenerator
{
private:
    const QuaterList &quater_list;
    RegisterManager registerManager;
    StackManager stackManager;

private:
    static const std::string TEMP_REG_PREFIX;

private:
    CodeList code_list;
    const std::string &func_name;

private:
    std::set<size_t> labels;

public:
    explicit MipsInstructionGenerator(const QuaterList &quaterList, const std::string &func_name = "") : quater_list(quaterList), registerManager(10, code_list), stackManager(code_list), func_name(func_name) {}

    void generator();

    void display_code_list(const std::string &file_path = "");

    CodeList get_code_list();

    static CodeList generate_header();

private:
    std::vector<std::string> context_list = {/* "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9",
                                            "$a0", "$a1", "$a2", "$a3", */
                                             "$fp", "$ra"};
    void save_context(const size_t &current_point);
    void load_context();

private:
    std::pair<size_t, size_t> get_interval(const std::string &var_id);

    void handleAssign(const Quater &quater, const size_t &index);

    void handleAdd(const Quater &quater, const size_t &index);

    void handleMinus(const Quater &quater, const size_t &index);

    void handleMul(const Quater &quater, const size_t &index);

    void handleDiv(const Quater &quater, const size_t &index);

    void handleMod(const Quater &quater, const size_t &index);

    Avalue alloc_a_variable(const Quater &quater, const std::string &arg, const size_t &index, const std::string &meta);

    std::pair<Avalue, Avalue>
    alloc_variable_pair(const Quater &quater, const std::string &arg1, const std::string &arg2,
                        const size_t &index,
                        const std::string &meta1, const std::string &meta2);

    void handleReturn(const Quater &quater, const size_t &index);

    void handleParam(const Quater &quater, const size_t &index);

    void handleCall(const Quater &quater, const size_t &index);

    void handleJump(const Quater &quater, const size_t &index);
};
