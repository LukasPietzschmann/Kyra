#pragma once

#include <utility>

#include "../Token.hpp"
#include "Expression.hpp"

namespace Kyra {
class AccessExpr : public Expression {
public:
	AccessExpr(const Position& position, const Expression::Ptr& owner, Token name) :
		Expression(position), m_owner(owner), m_name(std::move(name)) {}
	~AccessExpr() override = default;
	void accept(ExpressionVisitor& visitor) override { return visitor.visitAccessExpr(*this); }
	Expression::Ptr getOwner() const { return m_owner; }
	const Token& getName() const { return m_name; }

private:
	Expression::Ptr m_owner;
	Token m_name;
};
}