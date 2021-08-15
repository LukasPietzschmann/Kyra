#pragma once

#include "Expression.hpp"
#include "../Token.hpp"

namespace LibSlg {
class BinaryExpr : public Expression {
public:
	BinaryExpr(Expression::Ptr lhs, const Token& oper, Expression::Ptr rhs) :
			m_lhs(std::move(lhs)), m_operator(oper), m_rhs(std::move(rhs)) {}
	void accept(ExpressionVisitor& visitor) override { visitor.visitBinaryExpr(*this); }
	const Token& getOperator() const { return m_operator; }
	const Ptr& getLhs() const { return m_lhs; }
	const Ptr& getRhs() const { return m_rhs; }
private:
	Token m_operator;
	Expression::Ptr m_lhs;
	Expression::Ptr m_rhs;
};
}