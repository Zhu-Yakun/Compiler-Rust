#pragma once

enum TokenType
{
    // 基本类型
    T_IDENTIFIER,      // 标识符
    T_INTEGER_LITERAL, // 数值（整数）
    T_FLOAT_LITERAL,   // 浮点数（若有）
    T_STRING_LITERAL,  // 字符串 "hello"
    T_CHAR_LITERAL,    // 字符 'a'

    // 关键字
    T_I32,
    T_F32,
    T_BOOL,
    T_CHAR,

    T_LET,
    T_IF,
    T_ELSE,
    T_WHILE,
    T_RETURN,
    T_MUT,
    T_FN,
    T_FOR,
    T_IN,
    T_LOOP,
    T_BREAK,
    T_CONTINUE,

    // 运算符
    T_ASSIGN,       // =
    T_PLUS,         // +
    T_MINUS,        // -
    T_MULTIPLY,     // *
    T_REFERENCE,    // &
    T_DIVIDE,       // /
    T_EQUAL,        // ==
    T_GREATER,      // >
    T_GREATEREQUAL, // >=
    T_LESS,         // <
    T_LESSEQUAL,    // <=
    T_NOTEQUAL,     // !=

    // 界符
    T_LEFT_PAREN,   // (
    T_RIGHT_PAREN,  // )
    T_LEFT_BRACE,   // {
    T_RIGHT_BRACE,  // }
    T_LEFT_SQUARE,  // [
    T_RIGHT_SQUARE, // ]
    T_COMMA,        // ,

    // 分隔符
    T_SEMICOLON, // ;
    T_COLON,     // :

    // 特殊符号
    T_ARROW,  // ->
    T_DOT,    // .
    T_DOTDOT, // ..

    // 注释
    T_LINE_COMMENT,  // //
    T_BLOCK_COMMENT, // /* */

    // 文件结束或非法
    T_EOF,     // 文件结束
    T_UNKNOWN, // 未知Token
};