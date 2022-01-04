#pragma once

#include <ostream>
#include <string>
#include <utility>

#include "Position.hpp"
#include "TokenType.hpp"

namespace Kyra {
class Token {
public:
	struct TokenValue {
		explicit TokenValue(std::string val);

		std::string value;

		int asInt() const;
		std::string asString() const;
	};
	Token(const TokenType& type, Position position, std::string lexeme, std::string value = "");
	TokenType getType() const;
	const Position& getPosition() const;
	std::string getLexeme() const;
	TokenValue getValue() const;

	bool operator==(const Token& other) const;

	friend std::ostream& operator<<(std::ostream& os, const Token& rhs);

private:
	const TokenType m_type;
	const Position m_position;
	const std::string m_lexeme;
	const TokenValue m_value;
};
}