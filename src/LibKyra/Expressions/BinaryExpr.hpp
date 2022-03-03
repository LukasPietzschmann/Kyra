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
	const Token& get_operator() const;
	Expression::Ptr get_lhs() const;
	Expression::Ptr get_rhs() const;

private:
	Token m_operator;
	Expression::Ptr m_lhs;
	Expression::Ptr m_rhs;
};
}