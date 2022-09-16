#pragma once

#include <string>
#include <vector>

#include "SourceRange.hpp"

namespace Kyra {

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
	PRINT,
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
		int as_int() const;

	private:
		std::string_view m_value;
	};

	static std::string get_name_for(const TokenType& type) {
		static const std::vector<std::string> names{"(", ")", "{", "}", ",", ";", ":", "-", "+", "*", "\\", "=",
			"Identifier", "Number", "var", "val", "fun", "print", "return", "EOF"};
		return "\"" + names.at(static_cast<unsigned>(type)) + "\"";
	}

	Token(TokenType type, std::string_view lexeme, std::string_view literal_value, const SourceRange& source_range);

	TokenType get_type() const;
	const std::string_view get_lexeme() const;
	const LiteralValue& get_literal_value() const;
	const SourceRange& get_source_range() const;

private:
	const TokenType m_type;
	const std::string_view m_lexeme;
	const LiteralValue m_literal_value;
	const SourceRange m_source_range;
};
}
