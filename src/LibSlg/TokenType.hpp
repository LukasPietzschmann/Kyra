#pragma once

namespace LibSlg {
enum class TokenType {
	// Single-character tokens
	LEFT_PAREN, RIGHT_PAREN, LEFT_CURLY, RIGHT_CURLY,
	COMMA, SEMICOLON, DOT, COLON, MINUS, PLUS, SLASH, STAR,

	// One or two character tokens
	EQUAL, EQUAL_EQUAL,
	BANG, BANG_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,

	//Literals
	NAME, STRING, NUMBER,

	//Keywords
	VAR, VAL, FUN, OBJECT, PRINT, NOTHING, TRUE, FALSE, RETURN,

	END_OF_FILE
};

class TokenTypeName {
public:
	static std::string getFor(TokenType type) {
		const std::string names[] = {"LEFT_PAREN", "RIGHT_PAREN", "LEFT_CURLY", "RIGHT_CURLY", "COMMA", "SEMICOLON",
				"DOT", "COLON", "MINUS", "PLUS", "SLASH", "STAR", "EQUAL", "EQUAL_EQUAL", "BANG", "BANG_EQUAL", "GREATER",
				"GREATER_EQUAL", "LESS", "LESS_EQUAL", "NAME", "STRING", "NUMBER", "VAR", "VAL", "FUN", "OBJECT", "PRINT",
				"NOTHING", "TRUE", "FALSE", "RETURN", "EOF"};
		return names[static_cast<std::underlying_type<TokenType>::type>(type)];
	}
};

}