#include "UnaryExpr.hpp"

#include <utility>

namespace Kyra {
struct Position;
UnaryExpr::UnaryExpr(const Position& position, Token oper, const Expression::Ptr& rhs) :
	Expression(position), m_operator(std::move(oper)), m_rhs(rhs) {}

void UnaryExpr::accept(ExpressionVisitor& visitor) { return visitor.visit_unary_expr(*this); }

const Token& UnaryExpr::get_operator() const { return m_operator; }

Expression::Ptr UnaryExpr::get_rhs() const { return m_rhs; }
}