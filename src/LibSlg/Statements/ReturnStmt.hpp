#pragma once

#include "Statement.hpp"

class ReturnStmt : public Statement {
public:
	explicit ReturnStmt(Expression::Ptr expr) : m_expr(std::move(expr)) {}
private:
	Expression::Ptr m_expr;
};