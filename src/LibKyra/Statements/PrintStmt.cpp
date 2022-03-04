#include "PrintStmt.hpp"

namespace Kyra {
struct Position;
PrintStmt::PrintStmt(const Position& position, Expression::Ptr expr) : Statement(position), m_expr(std::move(expr)) {}

void PrintStmt::accept(StatementVisitor& visitor) { visitor.visit_print_stmt(*this); }

Expression::Ptr PrintStmt::get_expr() const { return m_expr; }
}