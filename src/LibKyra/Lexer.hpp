#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Result.hpp"
#include "Token.hpp"
#include "TokenType.hpp"

namespace Kyra {
class Lexer {
public:
	explicit Lexer(std::string source) : m_source(std::move(source)) {}
	const Result<std::vector<Token>>& scan_tokens();

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
			{"while", TokenType::WHILE},
			{"if", TokenType::IF},
			{"else", TokenType::ELSE}};
	const std::string m_source;
	Result<std::vector<Token>> m_result;
	Position::PositionPart m_start_position{1, 0};
	unsigned int m_current_character{0};
	unsigned int m_current_line{1};
	unsigned int m_character_at_start_line_start{0};
	unsigned int m_character_at_current_line_start{0};

	void scan_token();
	void comment();
	void number();
	void string();
	void name_or_keyword();
	char advance();
	char peek() const;
	std::string peek_multiple(int n) const;
	bool match(char expected) const;
	bool match_and_advance(char expected);
	bool match_multiple(const std::string& expected) const;
	bool match_multiple_and_advance(const std::string& expected);
	void add_token(TokenType type, const std::string& literal = "");
	static bool is_digit(char character);
	static bool is_alpha(char character);
	static bool is_whitespace(char character);
	bool is_at_end() const;
};
}