#include "BinaryExpr.hpp"

#include <utility>

namespace Kyra {
struct Position;
BinaryExpr::BinaryExpr(const Position& position, const Expression::Ptr& lhs, Token oper, const Expression::Ptr& rhs) :
	Expression(position), m_operator(std::move(oper)), m_lhs(lhs), m_rhs(rhs) {}

void BinaryExpr::accept(ExpressionVisitor& visitor) { return visitor.visitBinaryExpr(*this); }

const Token& BinaryExpr::getOperator() const { return m_operator; }

Expression::Ptr BinaryExpr::getLhs() const { return m_lhs; }

Expression::Ptr BinaryExpr::getRhs() const { return m_rhs; }
}