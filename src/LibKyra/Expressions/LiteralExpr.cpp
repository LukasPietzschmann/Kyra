#include "LiteralExpr.hpp"

#include <cassert>
#include <utility>

namespace Kyra {
struct Position;
LiteralExpr::LiteralExpr(const Position& position, int int_value) :
	Expression(position), m_type(t_int), m_literal(int_value) {}

LiteralExpr::LiteralExpr(const Position& position, std::string&& string_value) :
	Expression(position), m_type(t_string), m_literal(std::move(string_value)) {}

LiteralExpr::LiteralExpr(const Position& position, bool bool_value) :
	Expression(position), m_type(t_bool), m_literal(bool_value) {}

LiteralExpr::LiteralExpr(const Position& position) : Expression(position), m_type(t_nothing) {}

void LiteralExpr::accept(ExpressionVisitor& visitor) { return visitor.visit_literal(*this); }

LiteralExpr::Type LiteralExpr::get_type() const { return m_type; }

int LiteralExpr::get_int_value() const {
	assert(m_type == t_int);
	return std::get<int>(m_literal);
}

const std::string& LiteralExpr::get_string_value() const {
	assert(m_type == t_string);
	return std::get<std::string>(m_literal);
}

bool LiteralExpr::get_bool_value() const {
	assert(m_type == t_bool);
	return std::get<bool>(m_literal);
}
}