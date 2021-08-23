#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "Exceptions.hpp"
#include "Token.hpp"

namespace LibSlg {
class Lexer {
public:
	explicit Lexer(std::string source) : m_source(std::move(source)) {}
	std::vector<Token> scanTokens();
private:
	const std::unordered_map<std::string, TokenType> keywords {
			{"var", TokenType::VAR},
			{"val", TokenType::VAL},
			{"fun", TokenType::FUN},
			{"object", TokenType::OBJECT},
			{"print", TokenType::PRINT},
			{"nothing", TokenType::NOTHING},
			{"true", TokenType::TRUE},
			{"false", TokenType::FALSE},
			{"return", TokenType::RETURN},
	};
	const std::string m_source;
	std::vector<Token> m_tokens;
	int m_current {};
	int m_start {};
	int m_line {};

	void scanToken();
	void comment();
	void number();
	void string();
	void nameOrKeyword();
	char advance();
	char peek() const;
	bool match(char expected) const;
	bool matchAndAdvance(char expected);
	void addToken(TokenType type, const std::string& literal = "");
	bool isDigit(char character);
	bool isAlpha(char character);
	bool isAtEnd() const;
};
}