#pragma once

namespace Kyra {
enum class TokenType {
	// clang-format off
	// Single-character tokens
	LEFT_PAREN, RIGHT_PAREN, LEFT_CURLY, RIGHT_CURLY,
	COMMA, SEMICOLON, DOT, COLON, MINUS, PLUS, SLASH, STAR,

	// One or two character tokens
	EQUAL, EQUAL_EQUAL,
	BANG, BANG_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,
	ARROW,

	//Literals
	NAME, STRING, NUMBER,

	//Keywords
	VAR, VAL, FUN, CLASS, PRINT, NOTHING, TRUE, FALSE, RETURN, INSTANTIATE,
	WHILE,

	END_OF_FILE
	// clang-format on
};

class TokenTypeName {
public:
	static std::string get_for(TokenType type) {
		const std::string names[] = {"LEFT_PAREN",
				"RIGHT_PAREN",
				"LEFT_CURLY",
				"RIGHT_CURLY",
				"COMMA",
				"SEMICOLON",
				"DOT",
				"COLON",
				"MINUS",
				"PLUS",
				"SLASH",
				"STAR",
				"EQUAL",
				"EQUAL_EQUAL",
				"BANG",
				"BANG_EQUAL",
				"GREATER",
				"GREATER_EQUAL",
				"LESS",
				"LESS_EQUAL",
				"ARROW",
				"NAME",
				"STRING",
				"NUMBER",
				"VAR",
				"VAL",
				"FUN",
				"CLASS",
				"PRINT",
				"NOTHING",
				"TRUE",
				"FALSE",
				"RETURN",
				"INSTANTIATE",
				"WHILE",
				"EOF"};
		return names[static_cast<std::underlying_type_t<TokenType>>(type)];
	}
};

}