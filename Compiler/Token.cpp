#include "Token.hpp"

#include <cassert>
#include <stdexcept>

#include "Aliases.hpp"

namespace Kyra {

Token::LiteralValue::LiteralValue(std::string_view value) : m_value(value) {}

const std::string_view Token::LiteralValue::as_string() const { return m_value; }

int Token::LiteralValue::as_int() const {
	int res;
	try {
		res = std::stoi(std::string(m_value));
	} catch(const std::exception&) { // std::invalid_argument, std::out_of_range
		assert_not_reached();
	}
	return res;
}

Token::Token(TokenType type, std::string_view lexeme, std::string_view literal_value, const SourceRange& source_range) :
	m_type(type), m_lexeme(lexeme), m_literal_value(literal_value), m_source_range(source_range) {}

TokenType Token::get_type() const { return m_type; }

const std::string_view Token::get_lexeme() const { return m_lexeme; }

const Token::LiteralValue& Token::get_literal_value() const { return m_literal_value; }

const SourceRange& Token::get_source_range() const { return m_source_range; }
}
