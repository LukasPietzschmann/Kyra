#pragma once

#include "Statement.hpp"

namespace Slanguage {
class BlockStmt : public Statement {
public:
	BlockStmt(const Position& position, std::vector<Statement::Ptr> statements) :
		Statement(position), m_statements(std::move(statements)) {}
	~BlockStmt() override = default;
	void accept(StatementVisitor& visitor) override { visitor.visitBlockStmt(*this); }
	const std::vector<Statement::Ptr>& getStatements() const { return m_statements; }

private:
	std::vector<Statement::Ptr> m_statements;
};
}