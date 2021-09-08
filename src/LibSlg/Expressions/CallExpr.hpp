#pragma once

#include <utility>
#include <vector>

#include "Expression.hpp"

namespace LibSlg {
class CallExpr : public Expression {
public:
	CallExpr(Expression::Ptr function, std::vector<Expression::Ptr> arguments) :
		m_function(std::move(function)), m_arguments(std::move(arguments)) {}
	~CallExpr() override = default;
	void accept(ExpressionVisitor& visitor) override { return visitor.visitCallExpr(*this); }
	const Ptr& getFunction() const { return m_function; }
	const std::vector<Expression::Ptr>& getArguments() const { return m_arguments; }

private:
	Expression::Ptr m_function;
	std::vector<Expression::Ptr> m_arguments;
};
}