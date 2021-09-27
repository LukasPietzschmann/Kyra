#pragma once

#include <vector>

#include "Expression.hpp"

namespace LibSlg {
class InstantiationExpr : public Expression {
public:
	InstantiationExpr(const Position& position, Value::Type name, std::vector<Expression::Ptr> arguments) :
		Expression(position), m_name(std::move(name)), m_arguments(std::move(arguments)) {}
	~InstantiationExpr() override = default;
	void accept(ExpressionVisitor& visitor) override { return visitor.visitInstantiationExpr(*this); }

	const Value::Type& getName() const { return m_name; }
	const std::vector<Expression::Ptr>& getArguments() const { return m_arguments; }

private:
	Value::Type m_name;
	std::vector<Expression::Ptr> m_arguments;
};
}