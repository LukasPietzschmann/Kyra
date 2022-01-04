#include "PrintStmt.hpp"

namespace Kyra {
PrintStmt::PrintStmt(const Position& position, const Expression::Ptr& expr) : Statement(position), m_expr(expr) {}

void PrintStmt::accept(StatementVisitor& visitor) { visitor.visitPrintStmt(*this); }

Expression::Ptr PrintStmt::getExpr() const { return m_expr; }
}