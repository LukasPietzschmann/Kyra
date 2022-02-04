#pragma once

#include "Expression.hpp"

namespace Kyra {
struct Position;
class GroupExpr : public Expression {
public:
	GroupExpr(const Position& position, const Expression::Ptr& expr);
	~GroupExpr() override = default;

	Expression::Ptr getExpr() const;
	void accept(ExpressionVisitor& visitor) override;

private:
	Expression::Ptr m_expr;
};
}