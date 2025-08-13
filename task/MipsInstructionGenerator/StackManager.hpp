#pragma once

#include "CodeList.hpp"

class StackManager {
public:
    explicit StackManager(CodeList &codeList);

    void push(const std::string& reg_name) {
        code_list.emplace_back("sw " + reg_name + ", 0($sp)");
        code_list.emplace_back("addi $sp, $sp, -4");
    }

    void pop(const std::string& reg_name) {
        code_list.emplace_back("addi $sp, $sp, 4");
        code_list.emplace_back("lw " + reg_name + ", 0($sp)");
    }

    void push_list(const std::vector<std::string> &context_list);
    void pop_list(const std::vector<std::string> &context_list);

private:
    CodeList &code_list;

};


