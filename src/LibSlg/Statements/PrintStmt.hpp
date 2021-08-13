#pragma once

#include "Statement.hpp"

namespace LibSlg {
class PrintStmt : public Statement {
public:
	explicit PrintStmt(Expression::Ptr expr) : m_expr(std::move(expr)) {}
	void accept(StatementVisitor& visitor) override { visitor.visitPrintStmt(*this); }
private:
	Expression::Ptr m_expr;
};
}