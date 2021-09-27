#pragma once

#include "Statement.hpp"

namespace LibSlg {
class ExpressionStmt : public Statement {
public:
	ExpressionStmt(const Position& position, Expression::Ptr expr) : Statement(position), m_expr(std::move(expr)) {}
	~ExpressionStmt() override = default;
	void accept(StatementVisitor& visitor) override { visitor.visitExpressionStmt(*this); }
	const Expression::Ptr& getExpr() const { return m_expr; }

private:
	Expression::Ptr m_expr;
};
}