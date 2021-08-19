#pragma once

#include <utility>
#include "Expression.hpp"
#include "../Token.hpp"

namespace LibSlg {
class AccessExpr : public Expression {
public:
	AccessExpr(Expression::Ptr owner, Token name) : m_owner(std::move(owner)), m_name(std::move(name)) {}
	~AccessExpr() override {}
	Value::Ptr accept(ExpressionVisitor& visitor) override { return visitor.visitAccessExpr(*this); }
	const Expression::Ptr& getOwner() const { return m_owner; }
	const Token& getName() const { return m_name; }
private:
	Expression::Ptr m_owner;
	Token m_name;
};
}