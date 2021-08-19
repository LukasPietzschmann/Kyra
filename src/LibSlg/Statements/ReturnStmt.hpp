#pragma once

#include "Statement.hpp"

namespace LibSlg {
class ReturnStmt : public Statement {
public:
	explicit ReturnStmt(Expression::Ptr expr) : m_expr(std::move(expr)) {}
	~ReturnStmt() override {}
	void accept(StatementVisitor& visitor) override { visitor.visitReturnStmt(*this); }
	const Expression::Ptr& getExpr() const { return m_expr; }
private:
	Expression::Ptr m_expr;
};
}