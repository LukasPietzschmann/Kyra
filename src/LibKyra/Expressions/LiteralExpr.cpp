#include "LiteralExpr.hpp"

namespace Kyra {
LiteralExpr::LiteralExpr(const Position& position, const Value::Ptr& value) : Expression(position), m_value(value) {}

void LiteralExpr::accept(ExpressionVisitor& visitor) { return visitor.visitLiteral(*this); }

Value::Ptr LiteralExpr::getValue() const { return m_value; }
}