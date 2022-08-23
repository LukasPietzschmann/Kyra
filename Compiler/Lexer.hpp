#pragma once

#include <optional>
#include <string>
#include <vector>

#include "SourceRange.hpp"
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
	SourceRange::Position m_current{0, 0, 0};
	SourceRange::Position m_start{0, 0, 0};

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

	void line_brak();
};
