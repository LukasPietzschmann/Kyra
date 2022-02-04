#include "WhileStmt.hpp"

namespace Kyra {
struct Position;
WhileStmt::WhileStmt(const Position& position, Expression::Ptr condition, Statement::Ptr stmt) :
	Statement(position), m_condition(condition), m_stmt(stmt) {}

void WhileStmt::accept(StatementVisitor& visitor) { visitor.visitWhileStmt(*this); }

Expression::Ptr WhileStmt::getCondition() const { return m_condition; }
Statement::Ptr WhileStmt::getStatement() const { return m_stmt; }
}