#include "VariableExpr.hpp"

#include <utility>

namespace Kyra {
struct Position;
VariableExpr::VariableExpr(const Position& position, Token name) : Expression(position), m_name(std::move(name)) {}

void VariableExpr::accept(ExpressionVisitor& visitor) { return visitor.visitVariable(*this); }

const Token& VariableExpr::getName() const { return m_name; }
}