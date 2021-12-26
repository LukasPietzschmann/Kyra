#pragma once

#include <utility>

#include "../Token.hpp"
#include "Expression.hpp"

namespace Kyra {
class UnaryExpr : public Expression {
public:
	UnaryExpr(const Position& position, Token oper, const Expression::Ptr& rhs) :
		Expression(position), m_operator(std::move(oper)), m_rhs(rhs) {}
	~UnaryExpr() override = default;
	void accept(ExpressionVisitor& visitor) override { return visitor.visitUnaryExpr(*this); }
	const Token& getOperator() const { return m_operator; }
	Expression::Ptr getRhs() const { return m_rhs; }

private:
	Token m_operator;
	Expression::Ptr m_rhs;
};
}