#pragma once

#include "../Expressions/Expression.hpp"
#include "Statement.hpp"

namespace Kyra {
struct Position;
class WhileStmt : public Statement {
public:
	WhileStmt(const Position& position, Expression::Ptr condition, Statement::Ptr stmt);
	~WhileStmt() override = default;

	void accept(StatementVisitor& visitor) override;

	Expression::Ptr get_condition() const;
	Statement::Ptr get_statement() const;

private:
	Expression::Ptr m_condition;
	Statement::Ptr m_stmt;
};
}