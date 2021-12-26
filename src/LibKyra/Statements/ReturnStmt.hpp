#pragma once

#include "Statement.hpp"

namespace Kyra {
class ReturnStmt : public Statement {
public:
	ReturnStmt(const Position& position, const Expression::Ptr& expr) : Statement(position), m_expr(expr) {}
	~ReturnStmt() override = default;
	void accept(StatementVisitor& visitor) override { visitor.visitReturnStmt(*this); }
	Expression::Ptr getExpr() const { return m_expr; }

private:
	Expression::Ptr m_expr;
};
}