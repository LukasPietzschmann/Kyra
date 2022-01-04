#include "GroupExpr.hpp"

namespace Kyra {
GroupExpr::GroupExpr(const Position& position, const HasPtrAlias::Ptr& expr) : Expression(position), m_expr(expr) {}

Expression::Ptr GroupExpr::getExpr() const { return m_expr; }

void GroupExpr::accept(ExpressionVisitor& visitor) { return visitor.visitGroupExpr(*this); }
}