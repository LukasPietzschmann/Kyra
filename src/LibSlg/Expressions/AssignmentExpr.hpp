#pragma once

#include <utility>
#include "Expression.hpp"
#include "../Token.hpp"

namespace LibSlg {
class AssignmentExpr : public Expression {
public:
	AssignmentExpr(Token name, Expression::Ptr newValue) : m_name(std::move(name)), m_newValue(std::move(newValue)) {}
	~AssignmentExpr() override {}
	Value::Ptr accept(ExpressionVisitor& visitor) override { return visitor.visitAssignmentExpr(*this); }
	const Token& getName() const { return m_name; }
	const Expression::Ptr& getNewValue() const { return m_newValue; }
private:
	Token m_name;
	Expression::Ptr m_newValue;
};
}