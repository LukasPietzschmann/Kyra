#include "Token.hpp"

#include <cassert>
#include <stdexcept>

Token::LiteralValue::LiteralValue(std::string_view value) : m_value(value) {}

const std::string_view Token::LiteralValue::as_string() const { return m_value; }

const int Token::LiteralValue::as_int() const {
	int res;
	try {
		std::stoi(std::string(m_value));
	} catch(std::exception) {  // std::invalid_argument, std::out_of_range
		assert(0);	// TODO: assert_not_reached
	}
	return res;
}

Token::Token(TokenType type, std::string_view lexeme, std::string_view literal_value) :
	m_type(type), m_lexeme(lexeme), m_literal_value(literal_value) {}

const TokenType Token::get_type() const { return m_type; }

const std::string_view Token::get_lexeme() const { return m_lexeme; }

const Token::LiteralValue& Token::get_literal_value() const { return m_literal_value; }
