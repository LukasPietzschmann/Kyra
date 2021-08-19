#pragma once

#include "Statement.hpp"

namespace LibSlg {
class PrintStmt : public Statement {
public:
	explicit PrintStmt(Expression::Ptr expr) : m_expr(std::move(expr)) {}
	~PrintStmt() override {}
	void accept(StatementVisitor& visitor) override { visitor.visitPrintStmt(*this); }
	const Expression::Ptr& getExpr() const { return m_expr; }
private:
	Expression::Ptr m_expr;
};
}