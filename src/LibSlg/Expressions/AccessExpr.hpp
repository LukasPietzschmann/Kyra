#pragma once

#include <utility>

#include "../Token.hpp"
#include "Expression.hpp"

namespace Slanguage {
class AccessExpr : public Expression {
public:
	AccessExpr(const Position& position, Expression::Ptr owner, Token name) :
		Expression(position), m_owner(std::move(owner)), m_name(std::move(name)) {}
	~AccessExpr() override = default;
	void accept(ExpressionVisitor& visitor) override { return visitor.visitAccessExpr(*this); }
	const Expression::Ptr& getOwner() const { return m_owner; }
	const Token& getName() const { return m_name; }

private:
	Expression::Ptr m_owner;
	Token m_name;
};
}