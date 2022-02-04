#include "ReturnStmt.hpp"

namespace Kyra {
struct Position;
ReturnStmt::ReturnStmt(const Position& position, const Expression::Ptr& expr) : Statement(position), m_expr(expr) {}

void ReturnStmt::accept(StatementVisitor& visitor) { visitor.visitReturnStmt(*this); }

Expression::Ptr ReturnStmt::getExpr() const { return m_expr; }
}