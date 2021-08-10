#pragma once

#include "Statement.hpp"

class ExpressionStmt : public Statement {
public:
	explicit ExpressionStmt(Expression::Ptr expr) : m_expr(std::move(expr)) {}
private:
	Expression::Ptr m_expr;
};