#pragma once

#include <optional>
#include <string>
#include <vector>

#include "Token.hpp"

class Lexer {
public:
	static Lexer& the() {
		static Lexer instance;
		return instance;
	}

	Lexer() = default;
	Lexer(const Lexer&) = delete;
	Lexer(Lexer&&) noexcept = default;

	Lexer& operator=(const Lexer&) = delete;
	Lexer& operator=(Lexer&&) noexcept = default;

	const std::vector<Token>& scan_input(std::string_view source);

private:
	std::vector<Token> m_tokens;
	std::string_view m_source{};
	unsigned m_current_character{0};
	unsigned m_start_character{m_current_character};

	void scan_token();

	void comment();
	void number();
	void name_or_keyword();

	char advance();
	char peek() const;
	bool match(char expected) const;
	bool match_and_advance(char expected);
	std::string_view peek_multiple(unsigned n) const;
	bool match_multiple(std::string_view expected) const;
	bool match_multiple_and_advance(std::string_view expected);

	bool is_digit(char character) const;
	bool is_alpha(char character) const;
	std::optional<TokenType> is_keyword(std::string_view string) const;
	bool is_at_end() const;
	void add_token(TokenType type, std::string_view literal = "");
};
