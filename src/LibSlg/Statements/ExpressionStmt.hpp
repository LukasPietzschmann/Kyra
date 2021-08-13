#pragma once

#include "Statement.hpp"

namespace LibSlg {
class ExpressionStmt : public Statement {
public:
	explicit ExpressionStmt(Expression::Ptr expr) : m_expr(std::move(expr)) {}
	void accept(StatementVisitor& visitor) override { visitor.visitExpressionStmt(*this); }
private:
	Expression::Ptr m_expr;
};
}