#pragma once

#include <utility>

#include "../Token.hpp"
#include "Expression.hpp"

namespace Kyra {
class AssignmentExpr : public Expression {
public:
	AssignmentExpr(const Position& position,
			const Expression::Ptr& owner,
			Token name,
			const Expression::Ptr& newValue) :
		Expression(position),
		m_owner(owner), m_name(std::move(name)), m_newValue(newValue) {}
	~AssignmentExpr() override = default;
	void accept(ExpressionVisitor& visitor) override { return visitor.visitAssignmentExpr(*this); }
	Expression::Ptr getOwner() const { return m_owner; }
	const Token& getName() const { return m_name; }
	Expression::Ptr getNewValue() const { return m_newValue; }

private:
	Expression::Ptr m_owner;
	Token m_name;
	Expression::Ptr m_newValue;
};
}