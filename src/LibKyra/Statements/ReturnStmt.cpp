#include "ReturnStmt.hpp"

namespace Kyra {
struct Position;
ReturnStmt::ReturnStmt(const Position& position, Expression::Ptr expr) : Statement(position), m_expr(std::move(expr)) {}

void ReturnStmt::accept(StatementVisitor& visitor) { visitor.visit_return_stmt(*this); }

Expression::Ptr ReturnStmt::get_expr() const { return m_expr; }
}