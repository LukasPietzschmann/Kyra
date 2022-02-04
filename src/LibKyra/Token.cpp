#include "Token.hpp"

#include <string>
#include <utility>

namespace Kyra {
Token::Token(const TokenType& type, Position position, std::string lexeme, std::string value) :
	m_type(type), m_position(position), m_lexeme(std::move(lexeme)), m_value(TokenValue(std::move(value))) {}

TokenType Token::getType() const { return m_type; }

const Position& Token::getPosition() const { return m_position; }

std::string Token::getLexeme() const { return m_lexeme; }

Token::TokenValue Token::getValue() const { return m_value; }

bool Token::operator==(const Token& other) const {
	return m_type == other.m_type && m_position == other.m_position && m_lexeme == other.m_lexeme &&
		   m_value.value == other.m_value.value;
}

std::ostream& operator<<(std::ostream& os, const Token& rhs) {
	os << "Type " << TokenTypeName::getFor(rhs.m_type);
	if(!rhs.m_lexeme.empty())
		os << " with lexeme " << rhs.m_lexeme;
	if(!rhs.m_value.value.empty())
		os << " and a value of " << rhs.m_value.value;
	os << " from " << rhs.m_position.start << " to " << rhs.m_position.end;
	return os;
}

Token::TokenValue::TokenValue(std::string val) : value(std::move(val)) {}

int Token::TokenValue::asInt() const { return std::stoi(value); }

std::string Token::TokenValue::asString() const { return value; }
}