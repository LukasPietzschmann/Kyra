#pragma once

#include "../Token.hpp"
#include "Expression.hpp"

namespace Kyra {
struct Position;
class BinaryExpr : public Expression {
public:
	BinaryExpr(const Position& position, const Expression::Ptr& lhs, Token oper, const Expression::Ptr& rhs);
	~BinaryExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	const Token& getOperator() const;
	Expression::Ptr getLhs() const;
	Expression::Ptr getRhs() const;

private:
	Token m_operator;
	Expression::Ptr m_lhs;
	Expression::Ptr m_rhs;
};
}