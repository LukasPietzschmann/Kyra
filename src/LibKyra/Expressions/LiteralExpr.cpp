#include "LiteralExpr.hpp"

#include <cassert>

namespace Kyra {
struct Position;
LiteralExpr::LiteralExpr(const Position& position, int int_value) :
	Expression(position), m_type(t_int), m_int_literal(int_value) {}

LiteralExpr::LiteralExpr(const Position& position, std::string&& string_value) :
	Expression(position), m_type(t_string), m_string_literal(string_value) {}

LiteralExpr::LiteralExpr(const Position& position, bool bool_value) :
	Expression(position), m_type(t_bool), m_bool_literal(bool_value) {}

LiteralExpr::LiteralExpr(const Position& position) : Expression(position), m_type(t_nothing) {}

LiteralExpr::~LiteralExpr() {
	switch(m_type) {
		case t_string:
			using std::string;
			m_string_literal.~string();
			break;
		case t_int:
		case t_bool:
		case t_nothing: break;
	}
}

void LiteralExpr::accept(ExpressionVisitor& visitor) { return visitor.visit_literal(*this); }

LiteralExpr::Type LiteralExpr::get_type() const { return m_type; }

int LiteralExpr::get_int_value() const {
	assert(m_type == t_int);
	return m_int_literal;
}

const std::string& LiteralExpr::get_string_value() const {
	assert(m_type == t_string);
	return m_string_literal;
}

bool LiteralExpr::get_bool_value() const {
	assert(m_type == t_bool);
	return m_bool_literal;
}
}