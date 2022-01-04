#pragma once

#include <vector>

#include "Statement.hpp"

namespace Kyra {
class BlockStmt : public Statement {
public:
	BlockStmt(const Position& position, std::vector<Statement::Ptr> statements);
	~BlockStmt() override = default;
	void accept(StatementVisitor& visitor) override;
	const std::vector<Statement::Ptr>& getStatements() const;

private:
	std::vector<Statement::Ptr> m_statements;
};
}