#pragma once

#include <string>

enum class TokenType {
	// Single-character tokens
	LEFT_PAREN,
	RIGHT_PAREN,
	LEFT_CURLY,
	RIGHT_CURLY,
	COMMA,
	SEMICOLON,
	COLON,
	MINUS,
	PLUS,
	STAR,
	SLASH,
	EQUAL,

	// Literals
	NAME,
	NUMBER,

	// Keywords
	VAR,
	VAL,
	FUN,
	RETURN,

	// Miscellaneous
	END_OF_FILE
};

class Token {
public:
	class LiteralValue {
	public:
		explicit LiteralValue(std::string_view value);

		const std::string_view as_string() const;
		const int as_int() const;

	private:
		std::string_view m_value;
	};

	Token(TokenType type, std::string_view lexeme, std::string_view literal_value = "");

	const TokenType get_type() const;
	const std::string_view get_lexeme() const;
	const LiteralValue& get_literal_value() const;

private:
	const TokenType m_type;
	const std::string_view m_lexeme;
	const LiteralValue m_literal_value;
};
