#include "MipsInstructionGenerator/StackManager.hpp"

StackManager::StackManager(CodeList &codeList) : code_list(codeList) {}

void StackManager::push_list(const std::vector<std::string> &context_list)
{
    if (context_list.empty())
        return;
    auto size = context_list.size();
    for (auto i = 0; i < size; i++)
    {
        std::stringstream ss;
        ss << "sw " << context_list[i] << ", ";
        if (i != 0)
            ss << "-";
        ss << i * 4 << "($sp)";
        code_list.emplace_back(ss.str());
    }
    code_list.emplace_back("addi $sp, $sp, -" + std::to_string(4 * size));
}

void StackManager::pop_list(const std::vector<std::string> &context_list)
{
    if (context_list.empty())
        return;
    int size = (int)context_list.size();
    code_list.emplace_back("addi $sp, $sp, " + std::to_string(4 * size));
    for (auto i = size - 1; i >= 0; i--)
    {
        std::stringstream ss;
        ss << "lw " << context_list[i] << ", ";
        if (i != 0)
            ss << "-";
        ss << i * 4 << "($sp)";
        code_list.emplace_back(ss.str());
    }
}