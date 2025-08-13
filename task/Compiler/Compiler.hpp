#pragma once

#include "Lexer/Lexer.hpp"
#include "LR1Parser/LR1Parser.hpp"
#include "Semantic/SemanticAnalyzer.hpp"
#include <filesystem>

namespace fs = std::filesystem;

class Compiler
{
public:
    explicit Compiler(const std::string &grammar_file_path, const std::string &input_file_path, std::string output_file_path, bool show_output);

    void compile();

private:
    std::string output_file_path;

    Lexer lexer;
    LR1Parser parser;
    SemanticAnalyzer semanticAnalyzer;
    bool show_output;

private:
    static bool needsReParsing(const std::string &grammar_file_path, const std::string &cache_time_path);

    static void saveLastModifiedTime(const std::string &grammar_file_path, const std::string &time_file_path);

    static std::time_t fileTimeToTimeT(const fs::file_time_type &ftime);
};
