#pragma once

#include "Expression.hpp"

namespace Kyra {
struct Position;
class GroupExpr : public Expression {
public:
	GroupExpr(const Position& position, Expression::Ptr expr);
	~GroupExpr() override = default;

	Expression::Ptr get_expr() const;
	void accept(ExpressionVisitor& visitor) override;

private:
	Expression::Ptr m_expr;
};
}