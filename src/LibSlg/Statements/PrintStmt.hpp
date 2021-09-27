#pragma once

#include "Statement.hpp"

namespace LibSlg {
class PrintStmt : public Statement {
public:
	PrintStmt(const Position& position, Expression::Ptr expr) : Statement(position), m_expr(std::move(expr)) {}
	~PrintStmt() override = default;
	void accept(StatementVisitor& visitor) override { visitor.visitPrintStmt(*this); }
	const Expression::Ptr& getExpr() const { return m_expr; }

private:
	Expression::Ptr m_expr;
};
}