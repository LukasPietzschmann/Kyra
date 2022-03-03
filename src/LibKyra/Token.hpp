#pragma once

#include <ostream>

#include "Position.hpp"
#include "TokenType.hpp"

namespace Kyra {
class Token {
public:
	struct TokenValue {
		explicit TokenValue(std::string val);

		std::string value;

		int as_int() const;
		std::string as_string() const;
	};
	Token(const TokenType& type, Position position, std::string lexeme, std::string value = "");
	TokenType get_type() const;
	const Position& get_position() const;
	std::string get_lexeme() const;
	TokenValue get_value() const;

	bool operator==(const Token& other) const;

	friend std::ostream& operator<<(std::ostream& os, const Token& rhs);

private:
	const TokenType m_type;
	const Position m_position;
	const std::string m_lexeme;
	const TokenValue m_value;
};
}