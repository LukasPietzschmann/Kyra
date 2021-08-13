#pragma once

#include "Statement.hpp"

namespace LibSlg {
class ReturnStmt : public Statement {
public:
	explicit ReturnStmt(Expression::Ptr expr) : m_expr(std::move(expr)) {}
	void accept(StatementVisitor& visitor) override { visitor.visitReturnStmt(*this); }
private:
	Expression::Ptr m_expr;
};
}