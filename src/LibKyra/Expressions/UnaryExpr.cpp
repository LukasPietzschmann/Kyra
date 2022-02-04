#include "UnaryExpr.hpp"

#include <utility>

namespace Kyra {
struct Position;
UnaryExpr::UnaryExpr(const Position& position, Token oper, const Expression::Ptr& rhs) :
	Expression(position), m_operator(std::move(oper)), m_rhs(rhs) {}

void UnaryExpr::accept(ExpressionVisitor& visitor) { return visitor.visitUnaryExpr(*this); }

const Token& UnaryExpr::getOperator() const { return m_operator; }

Expression::Ptr UnaryExpr::getRhs() const { return m_rhs; }
}