#include "BinaryExpr.hpp"

#include <utility>

namespace Kyra {
struct Position;
BinaryExpr::BinaryExpr(const Position& position, const Expression::Ptr& lhs, Token oper, const Expression::Ptr& rhs) :
	Expression(position), m_operator(std::move(oper)), m_lhs(lhs), m_rhs(rhs) {}

void BinaryExpr::accept(ExpressionVisitor& visitor) { return visitor.visit_binary_expr(*this); }

const Token& BinaryExpr::get_operator() const { return m_operator; }

Expression::Ptr BinaryExpr::get_lhs() const { return m_lhs; }

Expression::Ptr BinaryExpr::get_rhs() const { return m_rhs; }
}