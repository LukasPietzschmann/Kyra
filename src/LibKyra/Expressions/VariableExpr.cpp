#include "VariableExpr.hpp"

#include <utility>

namespace Kyra {
struct Position;
VariableExpr::VariableExpr(const Position& position, Token name) : Expression(position), m_name(std::move(name)) {}

void VariableExpr::accept(ExpressionVisitor& visitor) { return visitor.visit_variable(*this); }

const Token& VariableExpr::get_name() const { return m_name; }
}