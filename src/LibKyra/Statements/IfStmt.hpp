#pragma once

#include "../Expressions/Expression.hpp"
#include "Statement.hpp"

namespace Kyra {
class IfStmt : public Statement {
public:
	IfStmt(const Position& position, Expression::Ptr condition, Statement::Ptr then, Statement::Ptr else_if);
	~IfStmt() override = default;

	void accept(StatementVisitor& visitor) override;

	Expression::Ptr get_condition() const;
	Statement::Ptr get_then() const;
	Statement::Ptr get_else() const;

private:
	Expression::Ptr m_condition;
	Statement::Ptr m_then;
	Statement::Ptr m_else;
};
}