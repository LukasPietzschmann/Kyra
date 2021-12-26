#pragma once

#include <utility>
#include <vector>

#include "Expression.hpp"

namespace Kyra {
class CallExpr : public Expression {
public:
	CallExpr(const Position& position, const Expression::Ptr& function, std::vector<Expression::Ptr> arguments) :
		Expression(position), m_function(function), m_arguments(std::move(arguments)) {}
	~CallExpr() override = default;
	void accept(ExpressionVisitor& visitor) override { return visitor.visitCallExpr(*this); }
	Expression::Ptr getFunction() const { return m_function; }
	const std::vector<Expression::Ptr>& getArguments() const { return m_arguments; }

private:
	Expression::Ptr m_function;
	std::vector<Expression::Ptr> m_arguments;
};
}