#pragma once

#include <utility>

#include "../Token.hpp"
#include "Expression.hpp"

namespace LibSlg {
class BinaryExpr : public Expression {
public:
	BinaryExpr(Expression::Ptr lhs, Token oper, Expression::Ptr rhs) :
		m_operator(std::move(oper)), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
	~BinaryExpr() override {}
	void accept(ExpressionVisitor& visitor) override { return visitor.visitBinaryExpr(*this); }
	const Token& getOperator() const { return m_operator; }
	const Ptr& getLhs() const { return m_lhs; }
	const Ptr& getRhs() const { return m_rhs; }

private:
	Token m_operator;
	Expression::Ptr m_lhs;
	Expression::Ptr m_rhs;
};
}