#include "LiteralExpr.hpp"

namespace Kyra {
struct Position;
LiteralExpr::LiteralExpr(const Position& position, const Value::Ptr& value) : Expression(position), m_value(value) {}

void LiteralExpr::accept(ExpressionVisitor& visitor) { return visitor.visit_literal(*this); }

Value::Ptr LiteralExpr::get_value() const { return m_value; }
}