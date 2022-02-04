#pragma once

#include "../Expressions/Expression.hpp"
#include "Statement.hpp"

namespace Kyra {
struct Position;
class ReturnStmt : public Statement {
public:
	ReturnStmt(const Position& position, const Expression::Ptr& expr);
	~ReturnStmt() override = default;

	void accept(StatementVisitor& visitor) override;
	Expression::Ptr getExpr() const;

private:
	Expression::Ptr m_expr;
};
}