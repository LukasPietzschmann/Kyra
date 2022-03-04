#include "GroupExpr.hpp"

namespace Kyra {
struct Position;
GroupExpr::GroupExpr(const Position& position, Expression::Ptr expr) : Expression(position), m_expr(std::move(expr)) {}

Expression::Ptr GroupExpr::get_expr() const { return m_expr; }

void GroupExpr::accept(ExpressionVisitor& visitor) { return visitor.visit_group_expr(*this); }
}