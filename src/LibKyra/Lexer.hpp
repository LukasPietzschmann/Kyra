#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Token.hpp"
#include "TokenType.hpp"

namespace Kyra {
class Lexer {
public:
	explicit Lexer(std::string source) : m_source(std::move(source)) {}
	std::vector<Token> scanTokens();

private:
	const std::unordered_map<std::string, TokenType> m_keywords{{"var", TokenType::VAR},
			{"val", TokenType::VAL},
			{"fun", TokenType::FUN},
			{"class", TokenType::CLASS},
			{"print", TokenType::PRINT},
			{"nothing", TokenType::NOTHING},
			{"true", TokenType::TRUE},
			{"false", TokenType::FALSE},
			{"return", TokenType::RETURN},
			{"instantiate", TokenType::INSTANTIATE},
			{"while", TokenType::WHILE}};
	const std::string m_source;
	std::vector<Token> m_tokens;
	int m_currentCharacter{};
	int m_startCharacter{};
	int m_currentLine{1};
	int m_startLine{1};
	int m_characterAtLineStart{};

	void scanToken();
	void comment();
	void number();
	void string();
	void nameOrKeyword();
	char advance();
	char peek() const;
	std::string peekMultiple(int n) const;
	bool match(char expected) const;
	bool matchAndAdvance(char expected);
	bool matchMultiple(const std::string& expected) const;
	bool matchMultipleAndAdvance(const std::string& expected);
	void addToken(TokenType type, const std::string& literal = "");
	bool isDigit(char character) const;
	bool isAlpha(char character) const;
	bool isWhitespace(char character) const;
	bool isAtEnd() const;
};
}