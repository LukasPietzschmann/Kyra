#include "WhileStmt.hpp"

namespace Kyra {
struct Position;
WhileStmt::WhileStmt(const Position& position, Expression::Ptr condition, Statement::Ptr stmt) :
	Statement(position), m_condition(condition), m_stmt(stmt) {}

void WhileStmt::accept(StatementVisitor& visitor) { visitor.visit_while_stmt(*this); }

Expression::Ptr WhileStmt::get_condition() const { return m_condition; }
Statement::Ptr WhileStmt::get_statement() const { return m_stmt; }
}