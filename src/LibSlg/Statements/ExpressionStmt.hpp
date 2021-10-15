#pragma once

#include "Statement.hpp"

namespace Slanguage {
class ExpressionStmt : public Statement {
public:
	ExpressionStmt(const Position& position, const Expression::Ptr& expr) : Statement(position), m_expr(expr) {}
	~ExpressionStmt() override = default;
	void accept(StatementVisitor& visitor) override { visitor.visitExpressionStmt(*this); }
	Expression::Ptr getExpr() const { return m_expr; }

private:
	Expression::Ptr m_expr;
};
}