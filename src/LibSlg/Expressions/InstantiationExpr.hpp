#pragma once

#include <vector>
#include "Expression.hpp"

namespace LibSlg {
class InstantiationExpr : public Expression {
public:
	InstantiationExpr(Value::Type name, std::vector<Expression::Ptr> arguments) :
			name(std::move(name)), m_arguments(std::move(arguments)) {}
	~InstantiationExpr() override {}
	Value::Ptr accept(ExpressionVisitor& visitor) override { return visitor.visitInstantiationExpr(*this); }

	const Value::Type& getName() const { return name; }
	const std::vector<Expression::Ptr>& getArguments() const { return m_arguments; }
private:
	Value::Type name;
	std::vector<Expression::Ptr> m_arguments;
};
}