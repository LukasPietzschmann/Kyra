#pragma once

#include "../Token.hpp"
#include "Expression.hpp"

namespace Kyra {
struct Position;
class AccessExpr : public Expression {
public:
	AccessExpr(const Position& position, Expression::Ptr owner, Token name);
	~AccessExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	Expression::Ptr get_owner() const;
	const Token& get_name() const;

private:
	Expression::Ptr m_owner;
	Token m_name;
};
}