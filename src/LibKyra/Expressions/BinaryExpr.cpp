#include "BinaryExpr.hpp"

#include <utility>

namespace Kyra {
struct Position;
BinaryExpr::BinaryExpr(const Position& position, Expression::Ptr lhs, Token oper, Expression::Ptr rhs) :
	Expression(position), m_operator(std::move(oper)), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

void BinaryExpr::accept(ExpressionVisitor& visitor) { return visitor.visit_binary_expr(*this); }

const Token& BinaryExpr::get_operator() const { return m_operator; }

Expression::Ptr BinaryExpr::get_lhs() const { return m_lhs; }

Expression::Ptr BinaryExpr::get_rhs() const { return m_rhs; }
}