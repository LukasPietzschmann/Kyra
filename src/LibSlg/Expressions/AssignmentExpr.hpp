#pragma once

#include <utility>

#include "../Token.hpp"
#include "Expression.hpp"

namespace LibSlg {
class AssignmentExpr : public Expression {
public:
	AssignmentExpr(const Position& position, Expression::Ptr owner, Token name, Expression::Ptr newValue) :
		Expression(position), m_owner(std::move(owner)), m_name(std::move(name)), m_newValue(std::move(newValue)) {}
	~AssignmentExpr() override = default;
	void accept(ExpressionVisitor& visitor) override { return visitor.visitAssignmentExpr(*this); }
	const Expression::Ptr& getOwner() const { return m_owner; }
	const Token& getName() const { return m_name; }
	const Expression::Ptr& getNewValue() const { return m_newValue; }

private:
	Expression::Ptr m_owner;
	Token m_name;
	Expression::Ptr m_newValue;
};
}