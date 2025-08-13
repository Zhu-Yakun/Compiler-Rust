#pragma once

#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "common/utils.hpp"

class CodeBlock
{
public:
    std::string code;
    bool top;

    explicit CodeBlock(std::string code, bool top = false)
        : code(std::move(code)), top(top) {}
};

class CodeList
{
public:
    std::vector<CodeBlock> code_list;

    void emplace_back(const std::string &str, const bool &top = false)
    {
        code_list.emplace_back(str, top);
    }

    void emplace_front(const std::string &str, const bool &top = false)
    {
        CodeBlock codeBlock(str, top);
        code_list.insert(code_list.begin() + 2, codeBlock);
    }

    void display(const std::string &path = "", bool show_in_terminal = true)
    {
        if (show_in_terminal)
        {
            for (const auto &block : code_list)
            {
                if (block.top)
                {
                    std::cout << block.code << std::endl;
                }
                else
                {
                    std::cout << "\t" << block.code << std::endl;
                }
            }
        }

        if (!path.empty())
        {
            std::ofstream fout(path, std::ios::out | std::ios::trunc);
            if (!fout.is_open())
            {
                std::cerr << "无法打开输出文件: " << path << std::endl;
                return;
            }
            for (const auto &block : code_list)
            {
                if (block.top)
                {
                    fout << block.code << std::endl;
                }
                else
                {
                    fout << "\t" << block.code << std::endl;
                }
            }
        }
    }

    void append(const CodeList &list)
    {
        code_list.insert(code_list.end(), list.code_list.begin(), list.code_list.end());
    }
};
