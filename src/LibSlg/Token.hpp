#pragma once

#include <string>
#include <ostream>
#include <utility>
#include "TokenType.hpp"

namespace LibSlg {
class Token {
public:
	Token(const TokenType& type, int line, std::string lexeme, std::string value = "") :
		m_type(type), m_line(line), m_lexeme(std::move(lexeme)), m_value(std::move(value)) {}
	TokenType getType() const { return m_type; }
	int getLine() const { return m_line; }
	std::string getLexeme() const { return m_lexeme; }
	std::string getValue() const { return m_value; }

	friend std::ostream& operator<<(std::ostream& os, const Token& token) {
		os << "Type " << TokenTypeName::getFor(token.m_type) << " at line " << token.m_line;
		if(!token.m_lexeme.empty())
			os << " with lexeme " << token.m_lexeme;
		if(!token.m_value.empty())
			os << " and a value of " << token.m_value;
		return os;
	}
private:
	const TokenType m_type;
	const int m_line;
	const std::string m_lexeme;
	const std::string m_value;
};
}