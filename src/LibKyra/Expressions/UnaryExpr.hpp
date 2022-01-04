#pragma once

#include <utility>

#include "../Token.hpp"
#include "Expression.hpp"

namespace Kyra {
class UnaryExpr : public Expression {
public:
	UnaryExpr(const Position& position, Token oper, const Expression::Ptr& rhs);
	~UnaryExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	const Token& getOperator() const;
	Expression::Ptr getRhs() const;

private:
	Token m_operator;
	Expression::Ptr m_rhs;
};
}