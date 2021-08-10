#pragma once

#include "Statement.hpp"

class PrintStmt : public Statement {
public:
	explicit PrintStmt(Expression::Ptr expr) : m_expr(std::move(expr)) {}
private:
	Expression::Ptr m_expr;
};