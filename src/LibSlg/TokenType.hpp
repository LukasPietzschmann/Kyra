#pragma once

namespace LibSlg {
enum class TokenType {
	// Single-character tokens
	LEFT_PAREN, RIGHT_PAREN, LEFT_CURLY, RIGHT_CURLY,
	COMMA, SEMICOLON, DOT, MINUS, PLUS, SLASH, STAR,

	// One or two character tokens
	EQUAL, EQUAL_EQUAL,
	BANG, BANG_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,

	//Literals
	NAME, STRING, NUMBER,

	//Keywords
	VAR, FUN, OBJECT, PRINT, NOTHING, TRUE, FALSE, RETURN,

	END_OF_FILE
};
}