#pragma once

#include <utility>

#include "../Token.hpp"
#include "Expression.hpp"

namespace Kyra {
class AssignmentExpr : public Expression {
public:
	AssignmentExpr(const Position& position, const Expression::Ptr& owner, Token name, const Expression::Ptr& newValue);
	~AssignmentExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	Expression::Ptr getOwner() const;
	const Token& getName() const;
	Expression::Ptr getNewValue() const;

private:
	Expression::Ptr m_owner;
	Token m_name;
	Expression::Ptr m_newValue;
};
}