#pragma once

#include "Token.hpp"
#include <string>
#include <vector>
#include <map>
#include <sstream>

class Lexer
{
private:
	std::string input;

	size_t index = 0;
	size_t col = 0;
	size_t row = 1;
	std::string current_line;
	std::string pre_line;

	std::vector<std::string> lines;

	char peek(int diff = 0);
	char getChar();
	void skipWhitespace();
	Token getIdentifierOrKeyword();
	Token getCharLiteral();
	Token getStringLiteral();
	Token getNumber();
	Token getOperator();
	Token getDelimiter();
	void skipComment();

	std::map<std::string, TokenType> operatorMap = {
		// 运算符
		{"=", T_ASSIGN},
		{"+", T_PLUS},
		{"-", T_MINUS},
		{"*", T_MULTIPLY},
		{"&", T_REFERENCE},
		{"/", T_DIVIDE},
		{"==", T_EQUAL},
		{">", T_GREATER},
		{">=", T_GREATEREQUAL},
		{"<", T_LESS},
		{"<=", T_LESSEQUAL},
		{"!=", T_NOTEQUAL},

		// 特殊符号
		{"->", T_ARROW},
		{"..", T_DOTDOT},
		{".", T_DOT}};

	std::map<std::string, TokenType> keywordMap = {
		{"i32", T_I32},
		{"let", T_LET},
		{"if", T_IF},
		{"else", T_ELSE},
		{"while", T_WHILE},
		{"return", T_RETURN},
		{"mut", T_MUT},
		{"fn", T_FN},
		{"for", T_FOR},
		{"in", T_IN},
		{"loop", T_LOOP},
		{"break", T_BREAK},
		{"continue", T_CONTINUE}

		// {"float", T_FLOAT},
		// {"char", T_CHAR},
		// {"struct", T_STRUCT},
		// ... 你可以根据需要添加其他关键字，例如 switch, case 等
	};

	std::map<std::string, TokenType> delimiterMap = {
		{";", T_SEMICOLON},
		{":", T_COLON},
		{"(", T_LEFT_PAREN},
		{")", T_RIGHT_PAREN},
		{"{", T_LEFT_BRACE},
		{"}", T_RIGHT_BRACE},
		{"[", T_LEFT_SQUARE},
		{"]", T_RIGHT_SQUARE},

		{",", T_COMMA},
		// {".", T_DOT},
		// {"->", T_ARROW},
		// {"?", T_QUESTION},
		// ... 其他界定符
	};

public:
	explicit Lexer(std::string input);
	explicit Lexer() = default;
	void setInput(std::string _input);
	Token getNextToken();
	std::string getCurrentRowCol();
};
