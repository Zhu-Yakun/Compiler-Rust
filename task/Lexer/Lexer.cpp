#include "Lexer.hpp"
#include <cctype>
#include <iostream>
#include <utility>

char Lexer::peek(int diff)
{
    if (index + diff < input.size())
    {
        return input[index + diff];
    }
    return '\0';
}

char Lexer::getChar()
{
    if (input[index] == '\n')
    {
        row++;
        col = 0;
        pre_line = current_line;
        current_line.clear();
    }
    else
    {
        col++;
        current_line += input[index];
    }
    return input[index++];
}

void Lexer::skipWhitespace()
{
    while (std::isspace(peek()))
    {
        getChar();
    }
}

Token Lexer::getIdentifierOrKeyword()
{
    std::string value;
    while (std::isalnum(peek()) || peek() == '_')
    {
        value += getChar();
    }

    if (keywordMap.count(value))
    {
        return {keywordMap[value], value};
    }
    return {T_IDENTIFIER, value};
}

Token Lexer::getOperator()
{
    std::string value;
    value += getChar();
    if (operatorMap.count(value + peek()))
    {
        value += getChar();
    }
    if (operatorMap.count(value))
    {
        return {operatorMap[value], value};
    }
    return {T_UNKNOWN, value};
}

Token Lexer::getStringLiteral()
{
    std::string value;
    value += getChar(); // Consume the opening quote
    while (peek() != '"' && peek() != '\0')
    {
        value += getChar();
    }
    value += getChar(); // Consume the closing quote
    return {T_STRING_LITERAL, value};
}

Token Lexer::getCharLiteral()
{
    std::string value;
    value += getChar(); // Consume the opening quote
    value += getChar(); // Consume the character
    value += getChar(); // Consume the closing quote
    return {T_CHAR_LITERAL, value};
}

Token Lexer::getNumber()
{
    std::string value;
    bool isFloat = false;

    while (std::isdigit(peek()) || (!isFloat && peek() == '.' && std::isdigit(peek(1))))
    {
        if (peek() == '.' && std::isdigit(peek(1)))
        {
            isFloat = true;
        }
        value += getChar();
    }

    // 检查数字后是否紧跟字母或下划线，如果是，则消耗直到非字母、非数字、非下划线的字符为止
    if (std::isalpha(peek()) || peek() == '_')
    {
        while (std::isalnum(peek()) || peek() == '_')
        {
            value += getChar();
        }
        return {T_UNKNOWN, value};
    }

    if (isFloat)
    {
        return {T_FLOAT_LITERAL, value};
    }
    else
    {
        return {T_INTEGER_LITERAL, value};
    }
}

Token Lexer::getDelimiter()
{
    std::string value;
    value += getChar();

    // 检查是否有双字符界定符，如 "->"
    if (delimiterMap.count(value + peek()))
    {
        value += getChar();
    }

    if (delimiterMap.count(value))
    {
        return {delimiterMap[value], value};
    }
    return {T_UNKNOWN, value};
}

void Lexer::skipComment()
{
    bool hasComment = false;
    if (peek() == '/')
    {
        if (input[index + 1] == '/')
        { // 单行注释
            hasComment = true;
            while (peek() != '\n' && peek() != '\0')
            {
                getChar();
            }
            if (peek() == '\n')
            {
                getChar(); // 消耗换行符
            }
        }
        else if (input[index + 1] == '*')
        {
            hasComment = true;
            // 多行注释
            getChar(); // 消耗/
            getChar(); // 消耗*
            while (!(peek() == '*' && input[index + 1] == '/'))
            {
                if (peek() == '\0')
                {
                    // 提示错误：多行注释没有正确关闭
                    std::cerr << "Error: Unclosed multi-line comment" << std::endl;
                    // exit(2);
                    throw std::exception();
                }
                getChar();
            }
            getChar(); // 消耗*
            getChar(); // 消耗/
        }
    }
    if (hasComment)
    {
        skipWhitespace();
        skipComment();
    }
}

Lexer::Lexer(std::string input) : input(std::move(input)) {}

Token Lexer::getNextToken()
{
    skipWhitespace();
    skipComment();
    if (index >= input.size())
    {
        return {T_EOF, ""};
    }

    char c = peek();
    // 标识符或关键字 字母或下划线开头
    if (std::isalpha(c) || c == '_')
    {
        return getIdentifierOrKeyword();
    }
    // 字符串或字符字面量 双引号开头，说明是字符串
    if (c == '"')
    {
        return getStringLiteral();
    }
    if (c == '\'') // 单引号开头，说明是字符
    {
        return getCharLiteral();
    }
    // 数字字面量 如果是数字或以.开头但后面是数字（例如 .5），识别为数字常量
    if (std::isdigit(c) || (c == '.' && index + 1 < input.size() && std::isdigit(input[index + 1])))
    {
        return getNumber();
    }
    // 双字符运算符判断（如 !=, ==, <=, >=）
    if (index + 1 < input.size())
    {
        std::string twoChar = input.substr(index, 2);
        if (operatorMap.count(twoChar))
        {
            index += 2;
            return {operatorMap[twoChar], twoChar};
        }
    }
    // 单字符运算符判断 如果是合法运算符的首字符（在 operatorMap 中）
    std::string oneChar(1, c);
    if (operatorMap.count(oneChar))
    {
        ++index;
        return {operatorMap[oneChar], oneChar};
    }
    // 分隔符 如果是括号、分号之类的符号，返回对应分隔符 token
    if (delimiterMap.count(oneChar))
    {
        ++index;
        return {delimiterMap[oneChar], oneChar};
    }
    // 无法识别的字符 如果都不匹配，读取一个字符并返回类型为 T_UNKNOWN 的 token，表示非法输入或尚未支持的语法
    return {T_UNKNOWN, std::string(1, getChar())};
}

void Lexer::setInput(std::string _input)
{
    this->input = std::move(_input);
    this->index = 0;
    this->row = 1;
    this->col = 0;
    this->lines.clear(); // 初始化

    std::stringstream ss(this->input);
    std::string line;
    while (std::getline(ss, line))
    {
        lines.push_back(line + '\n');
    }
}

std::string Lexer::getCurrentRowCol()
{
    std::stringstream ss;
    ss << "当前位置 row: " << row << " col: " << col << '\n';

    if (row - 1 >= lines.size())
        row = lines.size();
    if (row >= 2 && row - 2 < lines.size())
        ss << row - 1 << ": " << lines[row - 2];
    if (row - 1 < lines.size())
        ss << row << ": " << lines[row - 1];

    ss << std::string(std::to_string(row).size() + 2, ' ');
    ss << std::string(col > 0 ? col - 1 : 0, '-') << "^\n";

    return ss.str();
}
