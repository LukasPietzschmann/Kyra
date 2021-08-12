#pragma once

#include "Statement.hpp"

namespace LibSlg {
class ExpressionStmt : public Statement {
public:
	explicit ExpressionStmt(Expression::Ptr expr) : m_expr(std::move(expr)) {}
private:
	Expression::Ptr m_expr;
};
}