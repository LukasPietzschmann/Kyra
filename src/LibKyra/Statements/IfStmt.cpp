#include "IfStmt.hpp"
namespace Kyra {
IfStmt::IfStmt(const Position& position, Expression::Ptr condition, Statement::Ptr then, Statement::Ptr else_if) :
	Statement(position), m_condition(std::move(condition)), m_then(std::move(then)), m_else(std::move(else_if)) {}

void IfStmt::accept(Kyra::StatementVisitor& visitor) { visitor.visit_if_stmt(*this); }

Expression::Ptr IfStmt::get_condition() const { return m_condition; }

Statement::Ptr IfStmt::get_then() const { return m_then; }

Statement::Ptr IfStmt::get_else() const { return m_else; }
}