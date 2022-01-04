#include "BlockStmt.hpp"

namespace Kyra {
BlockStmt::BlockStmt(const Position& position, std::vector<Statement::Ptr> statements) :
	Statement(position), m_statements(std::move(statements)) {}

void BlockStmt::accept(StatementVisitor& visitor) { visitor.visitBlockStmt(*this); }

const std::vector<Statement::Ptr>& BlockStmt::getStatements() const { return m_statements; }
}