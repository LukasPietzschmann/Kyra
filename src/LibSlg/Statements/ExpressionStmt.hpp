#pragma once

#include "Statement.hpp"

namespace LibSlg {
class ExpressionStmt : public Statement {
public:
	explicit ExpressionStmt(Expression::Ptr expr) : m_expr(std::move(expr)) {}
	~ExpressionStmt() override {}
	void accept(StatementVisitor& visitor) override { visitor.visitExpressionStmt(*this); }
	const Expression::Ptr& getExpr() const { return m_expr; }
private:
	Expression::Ptr m_expr;
};
}