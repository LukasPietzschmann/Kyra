#pragma once

#include <utility>

#include "../Token.hpp"
#include "Expression.hpp"

namespace Kyra {
class BinaryExpr : public Expression {
public:
	BinaryExpr(const Position& position, const Expression::Ptr& lhs, Token oper, const Expression::Ptr& rhs) :
		Expression(position), m_operator(std::move(oper)), m_lhs(lhs), m_rhs(rhs) {}
	~BinaryExpr() override = default;
	void accept(ExpressionVisitor& visitor) override { return visitor.visitBinaryExpr(*this); }
	const Token& getOperator() const { return m_operator; }
	Expression::Ptr getLhs() const { return m_lhs; }
	Expression::Ptr getRhs() const { return m_rhs; }

private:
	Token m_operator;
	Expression::Ptr m_lhs;
	Expression::Ptr m_rhs;
};
}