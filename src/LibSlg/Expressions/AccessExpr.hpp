#pragma once

#include "Expression.hpp"
#include "../Token.hpp"

namespace LibSlg {
class AccessExpr : public Expression {
public:
	AccessExpr(Expression::Ptr owner, const Token& name) : m_owner(std::move(owner)), m_name(name) {}
	Value::Ptr accept(ExpressionVisitor& visitor) override { return visitor.visitAccessExpr(*this); }
	const Expression::Ptr& getOwner() const { return m_owner; }
	const Token& getName() const { return m_name; }
private:
	Expression::Ptr m_owner;
	Token m_name;
};
}