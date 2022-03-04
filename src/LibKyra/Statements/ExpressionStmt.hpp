#pragma once

#include "../Expressions/Expression.hpp"
#include "Statement.hpp"

namespace Kyra {
struct Position;
class ExpressionStmt : public Statement {
public:
	ExpressionStmt(const Position& position, Expression::Ptr expr);
	~ExpressionStmt() override = default;

	void accept(StatementVisitor& visitor) override;
	Expression::Ptr get_expr() const;

private:
	Expression::Ptr m_expr;
};
}