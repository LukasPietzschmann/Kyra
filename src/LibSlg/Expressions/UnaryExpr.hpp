#pragma once

#include "Expression.hpp"
#include "../Token.hpp"

namespace LibSlg {
class UnaryExpr : public Expression {
public:
	UnaryExpr(const Token& oper, Expression::Ptr rhs) : m_operator(oper), m_rhs(std::move(rhs)) {}
	Value::Ptr accept(ExpressionVisitor& visitor) override { return visitor.visitUnaryExpr(*this); }
	const Token& getOperator() const { return m_operator; }
	const Expression::Ptr& getRhs() const { return m_rhs; }
private:
	Token m_operator;
	Expression::Ptr m_rhs;
};
}