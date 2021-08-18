#pragma once

#include <string>
#include <ostream>
#include <utility>
#include "TokenType.hpp"

namespace LibSlg {
class Token {
public:
	typedef struct TokenValue {
		explicit TokenValue(std::string val) : value(std::move(val)) {};
		std::string value;
		int asInt() const { return std::stoi(value); };
		std::string asString() const { return value; };
	} TokenValue;
	Token(const TokenType& type, int line, std::string lexeme, std::string value = "") :
			m_type(type), m_line(line), m_lexeme(std::move(lexeme)), m_value(TokenValue(std::move(value))) {}
	TokenType getType() const { return m_type; }
	int getLine() const { return m_line; }
	std::string getLexeme() const { return m_lexeme; }
	TokenValue getValue() const { return m_value; }

	bool operator==(const Token& other) const {
		return m_type == other.m_type && m_line == other.m_line && m_lexeme == other.m_lexeme &&
				m_value.value == other.m_value.value;
	}

	friend std::ostream& operator<<(std::ostream& os, const Token& token) {
		os << "Type " << TokenTypeName::getFor(token.m_type) << " at line " << token.m_line;
		if(!token.m_lexeme.empty())
			os << " with lexeme " << token.m_lexeme;
		if(!token.m_value.value.empty())
			os << " and a value of " << token.m_value.value;
		return os;
	}
private:
	const TokenType m_type;
	const int m_line;
	const std::string m_lexeme;
	const TokenValue m_value;
};
}