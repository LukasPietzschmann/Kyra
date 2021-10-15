#pragma once

#include <utility>

#include "Expression.hpp"

namespace Slanguage {
class GroupExpr : public Expression {
public:
	GroupExpr(const Position& position, const Expression::Ptr& expr) : Expression(position), m_expr(expr) {}
	~GroupExpr() override = default;
	Expression::Ptr getExpr() const { return m_expr; }
	void accept(ExpressionVisitor& visitor) override { return visitor.visitGroupExpr(*this); }

private:
	Expression::Ptr m_expr;
};
}