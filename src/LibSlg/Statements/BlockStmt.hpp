#pragma once

#include "Statement.hpp"

namespace LibSlg {
class BlockStmt : public Statement {
public:
	explicit BlockStmt(std::vector<Statement::Ptr> statements) : m_statements(std::move(statements)) {}
	void accept(StatementVisitor& visitor) override { visitor.visitBlockStmt(*this); }
private:
	std::vector<Statement::Ptr> m_statements;
};
}