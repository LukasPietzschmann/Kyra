#pragma once

#include "Statement.hpp"

namespace LibSlg {
class BlockStmt : public Statement {
public:
	explicit BlockStmt(std::vector<Statement::Ptr> statements) : m_statements(std::move(statements)) {}
	~BlockStmt() override = default;
	void accept(StatementVisitor& visitor) override { visitor.visitBlockStmt(*this); }
	const std::vector<Statement::Ptr>& getStatements() const { return m_statements; }

private:
	std::vector<Statement::Ptr> m_statements;
};
}