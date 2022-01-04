#pragma once

#include <utility>
#include <vector>

#include "Expression.hpp"

namespace Kyra {
class CallExpr : public Expression {
public:
	CallExpr(const Position& position, const Expression::Ptr& function, std::vector<Expression::Ptr> arguments);
	~CallExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	Expression::Ptr getFunction() const;
	const std::vector<Expression::Ptr>& getArguments() const;

private:
	Expression::Ptr m_function;
	std::vector<Expression::Ptr> m_arguments;
};
}