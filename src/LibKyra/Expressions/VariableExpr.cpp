#include "VariableExpr.hpp"

namespace Kyra {
VariableExpr::VariableExpr(const Position& position, Token name) : Expression(position), m_name(std::move(name)) {}

void VariableExpr::accept(ExpressionVisitor& visitor) { return visitor.visitVariable(*this); }

const Token& VariableExpr::getName() const { return m_name; }
}