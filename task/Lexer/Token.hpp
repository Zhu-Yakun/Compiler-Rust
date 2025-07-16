#pragma once

#include <string>
#include <utility>
#include "TokenType.hpp"

struct Token
{
    Token(const TokenType &type, std::string value) : type(type), value(std::move(value)) {}

    Token() { type = T_UNKNOWN; };

    TokenType type;    // 属于哪个终结符
    std::string value; // 终结符的具体值

    [[nodiscard]] std::string type_to_string() const
    {
        return tokenTypeToString(type);
    }

public:
    static std::string tokenTypeToString(const TokenType &type)
    {
        switch (type)
        {
        case T_IDENTIFIER:
            return "T_IDENTIFIER";
        case T_INTEGER_LITERAL:
            return "T_INTEGER_LITERAL";
        case T_FLOAT_LITERAL:
            return "T_FLOAT_LITERAL";
        case T_STRING_LITERAL:
            return "T_STRING_LITERAL";
        case T_CHAR_LITERAL:
            return "T_CHAR_LITERAL";
        case T_I32:
            return "T_I32";
        case T_F32:
            return "T_F32";
        case T_LET:
            return "T_LET";
        case T_IF:
            return "T_IF";
        case T_ELSE:
            return "T_ELSE";
        case T_WHILE:
            return "T_WHILE";
        case T_RETURN:
            return "T_RETURN";
        case T_MUT:
            return "T_MUT";
        case T_FN:
            return "T_FN";
        case T_FOR:
            return "T_FOR";
        case T_IN:
            return "T_IN";
        case T_LOOP:
            return "T_LOOP";
        case T_BREAK:
            return "T_BREAK";
        case T_CONTINUE:
            return "T_CONTINUE";
        case T_ASSIGN:
            return "T_ASSIGN";
        case T_PLUS:
            return "T_PLUS";
        case T_MINUS:
            return "T_MINUS";
        case T_MULTIPLY:
            return "T_MULTIPLY";
        case T_REFERENCE:
            return "T_REFERENCE";
        case T_DIVIDE:
            return "T_DIVIDE";
        case T_EQUAL:
            return "T_EQUAL";
        case T_GREATER:
            return "T_GREATER";
        case T_GREATEREQUAL:
            return "T_GREATEREQUAL";
        case T_LESS:
            return "T_LESS";
        case T_LESSEQUAL:
            return "T_LESSEQUAL";
        case T_NOTEQUAL:
            return "T_NOTEQUAL";
        case T_LEFT_PAREN:
            return "T_LEFT_PAREN";
        case T_RIGHT_PAREN:
            return "T_RIGHT_PAREN";
        case T_LEFT_BRACE:
            return "T_LEFT_BRACE";
        case T_RIGHT_BRACE:
            return "T_RIGHT_BRACE";
        case T_LEFT_SQUARE:
            return "T_LEFT_SQUARE";
        case T_RIGHT_SQUARE:
            return "T_RIGHT_SQUARE";
        case T_SEMICOLON:
            return "T_SEMICOLON";
        case T_COLON:
            return "T_COLON";
        case T_ARROW:
            return "T_ARROW";
        case T_DOT:
            return "T_DOT";
        case T_DOTDOT:
            return "T_DOTDOT";
        case T_LINE_COMMENT:
            return "T_LINE_COMMENT";
        case T_BLOCK_COMMENT:
            return "T_BLOCK_COMMENT";
        case T_EOF:
            return "T_EOF";
        default:
            return "INVALID_TOKEN_TYPE";
        }
    }
    static TokenType stringToTokenType(const std::string &str)
    {
        if (str == "T_IDENTIFIER")
            return T_IDENTIFIER;
        else if (str == "T_INTEGER_LITERAL")
            return T_INTEGER_LITERAL;
        if (str == "T_FLOAT_LITERAL")
            return T_FLOAT_LITERAL;
        else if (str == "T_STRING_LITERAL")
            return T_STRING_LITERAL;
        else if (str == "T_CHAR_LITERAL")
            return T_CHAR_LITERAL;

        if (str == "T_I32")
            return T_I32;
        if (str == "T_F32")
            return T_F32;
        if (str == "T_BOOL")
            return T_BOOL;
        if (str == "T_CHAR")
            return T_CHAR;

        if (str == "T_LET")
            return T_LET;
        if (str == "T_IF")
            return T_IF;
        if (str == "T_ELSE")
            return T_ELSE;
        if (str == "T_WHILE")
            return T_WHILE;
        if (str == "T_RETURN")
            return T_RETURN;
        if (str == "T_MUT")
            return T_MUT;
        if (str == "T_FN")
            return T_FN;
        if (str == "T_FOR")
            return T_FOR;
        if (str == "T_IN")
            return T_IN;
        if (str == "T_LOOP")
            return T_LOOP;
        if (str == "T_BREAK")
            return T_BREAK;
        if (str == "T_CONTINUE")
            return T_CONTINUE;

        if (str == "T_ASSIGN")
            return T_ASSIGN;
        if (str == "T_PLUS")
            return T_PLUS;
        if (str == "T_MINUS")
            return T_MINUS;
        if (str == "T_MULTIPLY")
            return T_MULTIPLY;
        if (str == "T_REFERENCE")
            return T_REFERENCE;
        if (str == "T_DIVIDE")
            return T_DIVIDE;
        if (str == "T_EQUAL")
            return T_EQUAL;
        if (str == "T_GREATER")
            return T_GREATER;
        if (str == "T_GREATEREQUAL")
            return T_GREATEREQUAL;
        if (str == "T_LESS")
            return T_LESS;
        if (str == "T_LESSEQUAL")
            return T_LESSEQUAL;
        if (str == "T_NOTEQUAL")
            return T_NOTEQUAL;

        if (str == "T_LEFT_PAREN")
            return T_LEFT_PAREN;
        if (str == "T_RIGHT_PAREN")
            return T_RIGHT_PAREN;
        if (str == "T_LEFT_BRACE")
            return T_LEFT_BRACE;
        if (str == "T_RIGHT_BRACE")
            return T_RIGHT_BRACE;
        if (str == "T_LEFT_SQUARE")
            return T_LEFT_SQUARE;
        if (str == "T_RIGHT_SQUARE")
            return T_RIGHT_SQUARE;
        if (str == "T_COMMA")
            return T_COMMA;

        if (str == "T_SEMICOLON")
            return T_SEMICOLON;
        if (str == "T_COLON")
            return T_COLON;

        if (str == "T_ARROW")
            return T_ARROW;
        if (str == "T_DOT")
            return T_DOT;
        if (str == "T_DOTDOT")
            return T_DOTDOT;

        if (str == "T_LINE_COMMENT")
            return T_LINE_COMMENT;
        if (str == "T_BLOCK_COMMENT")
            return T_BLOCK_COMMENT;

        if (str == "T_EOF")
            return T_EOF;
        else
            return T_UNKNOWN; // Assuming INVALID_TOKEN_TYPE is a defined value for invalid inputs
    }
};
