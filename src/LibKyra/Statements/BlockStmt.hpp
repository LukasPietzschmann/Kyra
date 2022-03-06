#pragma once

#include <vector>

#include "Statement.hpp"

namespace Kyra {
struct Position;
class BlockStmt : public Statement {
public:
	BlockStmt(const Position& position, std::vector<Statement::Ptr>&& statements);
	~BlockStmt() override = default;
	void accept(StatementVisitor& visitor) override;
	const std::vector<Statement::Ptr>& get_statements() const;

private:
	std::vector<Statement::Ptr> m_statements;
};
}