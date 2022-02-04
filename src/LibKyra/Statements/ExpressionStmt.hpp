#pragma once

#include "../Expressions/Expression.hpp"
#include "Statement.hpp"

namespace Kyra {
struct Position;
class ExpressionStmt : public Statement {
public:
	ExpressionStmt(const Position& position, const Expression::Ptr& expr);
	~ExpressionStmt() override = default;

	void accept(StatementVisitor& visitor) override;
	Expression::Ptr getExpr() const;

private:
	Expression::Ptr m_expr;
};
}