#pragma once

#include "Statement.hpp"

namespace Kyra {
class PrintStmt : public Statement {
public:
	PrintStmt(const Position& position, const Expression::Ptr& expr) : Statement(position), m_expr(expr) {}
	~PrintStmt() override = default;
	void accept(StatementVisitor& visitor) override { visitor.visitPrintStmt(*this); }
	Expression::Ptr getExpr() const { return m_expr; }

private:
	Expression::Ptr m_expr;
};
}