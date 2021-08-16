#pragma once

#include "Expression.hpp"
#include "../Token.hpp"

namespace LibSlg {
class AssignmentExpr : public Expression {
public:
	AssignmentExpr(const Token& name, Expression::Ptr  newValue) : m_name(name), m_newValue(std::move(newValue)) {}
	void accept(ExpressionVisitor& visitor) override { visitor.visitAssignmentExpr(*this); }
	const Token& getName() const { return m_name; }
	const Expression::Ptr& getNewValue() const { return m_newValue; }
private:
	Token m_name;
	Expression::Ptr m_newValue;
};
}