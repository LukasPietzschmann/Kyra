#include "ExpressionStmt.hpp"

namespace Kyra {
struct Position;
ExpressionStmt::ExpressionStmt(const Position& position, Expression::Ptr expr) :
	Statement(position), m_expr(std::move(expr)) {}

void ExpressionStmt::accept(StatementVisitor& visitor) { visitor.visit_expression_stmt(*this); }

Expression::Ptr ExpressionStmt::get_expr() const { return m_expr; }
}