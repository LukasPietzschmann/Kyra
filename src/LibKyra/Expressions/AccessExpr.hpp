#pragma once

#include "../Token.hpp"
#include "Expression.hpp"

namespace Kyra {
struct Position;
class AccessExpr : public Expression {
public:
	AccessExpr(const Position& position, const Expression::Ptr& owner, Token name);
	~AccessExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	Expression::Ptr getOwner() const;
	const Token& getName() const;

private:
	Expression::Ptr m_owner;
	Token m_name;
};
}