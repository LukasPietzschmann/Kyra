#pragma once

#include <utility>
#include "Expression.hpp"
#include "../Token.hpp"

namespace LibSlg {
class UnaryExpr : public Expression {
public:
	UnaryExpr(Token oper, Expression::Ptr rhs) : m_operator(std::move(oper)), m_rhs(std::move(rhs)) {}
	~UnaryExpr() override {}
	void accept(ExpressionVisitor& visitor) override { return visitor.visitUnaryExpr(*this); }
	const Token& getOperator() const { return m_operator; }
	const Expression::Ptr& getRhs() const { return m_rhs; }
private:
	Token m_operator;
	Expression::Ptr m_rhs;
};
}