#pragma once

#include "../Expressions/Expression.hpp"
#include "Statement.hpp"

namespace Kyra {
struct Position;
class PrintStmt : public Statement {
public:
	PrintStmt(const Position& position, Expression::Ptr expr);
	~PrintStmt() override = default;

	void accept(StatementVisitor& visitor) override;
	Expression::Ptr get_expr() const;

private:
	Expression::Ptr m_expr;
};
}