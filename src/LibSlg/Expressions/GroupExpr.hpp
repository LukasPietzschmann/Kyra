#pragma once

#include <utility>
#include "Expression.hpp"

namespace LibSlg {
class GroupExpr : public Expression {
public:
	explicit GroupExpr(Expression::Ptr expr) : m_expr(std::move(expr)) {}
	~GroupExpr() override {}
	const Expression::Ptr& getExpr() const { return m_expr; }
	Value::Ptr accept(ExpressionVisitor& visitor) override { return visitor.visitGroupExpr(*this); }
private:
	Expression::Ptr m_expr;
};
}