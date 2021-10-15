#pragma once

#include <ostream>
#include <string>
#include <utility>

#include "Position.hpp"
#include "TokenType.hpp"

namespace Slanguage {
class Token {
public:
	struct TokenValue {
		explicit TokenValue(std::string val) : value(std::move(val)){};
		std::string value;
		int asInt() const { return std::stoi(value); };
		std::string asString() const { return value; };
	};
	Token(const TokenType& type, Position position, std::string lexeme, std::string value = "") :
		m_type(type), m_position(position), m_lexeme(std::move(lexeme)), m_value(TokenValue(std::move(value))) {}
	TokenType getType() const { return m_type; }
	const Position& getPosition() const { return m_position; }
	std::string getLexeme() const { return m_lexeme; }
	TokenValue getValue() const { return m_value; }

	bool operator==(const Token& other) const {
		return m_type == other.m_type && m_position == other.m_position && m_lexeme == other.m_lexeme &&
			   m_value.value == other.m_value.value;
	}

	friend std::ostream& operator<<(std::ostream& os, const Token& rhs) {
		os << "Type " << TokenTypeName::getFor(rhs.m_type);
		if(!rhs.m_lexeme.empty())
			os << " with lexeme " << rhs.m_lexeme;
		if(!rhs.m_value.value.empty())
			os << " and a value of " << rhs.m_value.value;
		os << " from " << rhs.m_position.start << " to " << rhs.m_position.end;
		return os;
	}

private:
	const TokenType m_type;
	const Position m_position;
	const std::string m_lexeme;
	const TokenValue m_value;
};
}