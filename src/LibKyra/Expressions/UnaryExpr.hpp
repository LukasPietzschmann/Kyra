#pragma once

#include "../Token.hpp"
#include "Expression.hpp"

namespace Kyra {
struct Position;
class UnaryExpr : public Expression {
public:
	UnaryExpr(const Position& position, Token oper, Expression::Ptr rhs);
	~UnaryExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	const Token& get_operator() const;
	Expression::Ptr get_rhs() const;

private:
	Token m_operator;
	Expression::Ptr m_rhs;
};
}