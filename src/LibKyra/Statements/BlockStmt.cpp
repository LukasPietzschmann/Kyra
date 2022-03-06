#include "BlockStmt.hpp"

#include <utility>

namespace Kyra {
struct Position;
BlockStmt::BlockStmt(const Position& position, std::vector<Statement::Ptr>&& statements) :
	Statement(position), m_statements(std::move(statements)) {}

void BlockStmt::accept(StatementVisitor& visitor) { visitor.visit_block_stmt(*this); }

const std::vector<Statement::Ptr>& BlockStmt::get_statements() const { return m_statements; }
}