#pragma once

#include "Statement.hpp"

namespace Slanguage {
class ReturnStmt : public Statement {
public:
	ReturnStmt(const Position& position, Expression::Ptr expr) : Statement(position), m_expr(std::move(expr)) {}
	~ReturnStmt() override = default;
	void accept(StatementVisitor& visitor) override { visitor.visitReturnStmt(*this); }
	const Expression::Ptr& getExpr() const { return m_expr; }

private:
	Expression::Ptr m_expr;
};
}