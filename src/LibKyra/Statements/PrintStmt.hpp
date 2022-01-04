#pragma once

#include "../Expressions/Expression.hpp"
#include "Statement.hpp"

namespace Kyra {
class PrintStmt : public Statement {
public:
	PrintStmt(const Position& position, const Expression::Ptr& expr);
	~PrintStmt() override = default;

	void accept(StatementVisitor& visitor) override;
	Expression::Ptr getExpr() const;

private:
	Expression::Ptr m_expr;
};
}