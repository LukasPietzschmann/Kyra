#pragma once

#include <utility>
#include "Expression.hpp"

namespace LibSlg {
class GroupExpr : public Expression {
public:
	explicit GroupExpr(Expression::Ptr expr) : m_expr(std::move(expr)) {}
	const Expression::Ptr& getExpr() const { return m_expr; }
	void accept(ExpressionVisitor& visitor) override { visitor.visitGroupExpr(*this); }
private:
	Expression::Ptr m_expr;
};
}