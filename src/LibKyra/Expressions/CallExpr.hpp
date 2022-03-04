#pragma once

#include <utility>
#include <vector>

#include "Expression.hpp"

namespace Kyra {
class CallExpr : public Expression {
public:
	CallExpr(const Position& position, Expression::Ptr function, std::vector<Expression::Ptr> arguments);
	~CallExpr() override = default;

	void accept(ExpressionVisitor& visitor) override;
	Expression::Ptr get_function() const;
	const std::vector<Expression::Ptr>& get_arguments() const;

private:
	Expression::Ptr m_function;
	std::vector<Expression::Ptr> m_arguments;
};
}