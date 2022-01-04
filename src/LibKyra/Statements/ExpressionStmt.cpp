#include "ExpressionStmt.hpp"

namespace Kyra {
ExpressionStmt::ExpressionStmt(const Position& position, const Expression::Ptr& expr) :
	Statement(position), m_expr(expr) {}

void ExpressionStmt::accept(StatementVisitor& visitor) { visitor.visitExpressionStmt(*this); }

Expression::Ptr ExpressionStmt::getExpr() const { return m_expr; }
}