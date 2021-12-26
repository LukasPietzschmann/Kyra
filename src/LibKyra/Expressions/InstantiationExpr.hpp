#pragma once

#include <vector>

#include "Expression.hpp"

namespace Kyra {
class InstantiationExpr : public Expression {
public:
	InstantiationExpr(const Position& position, std::string name, std::vector<Expression::Ptr> arguments) :
		Expression(position), m_name(std::move(name)), m_arguments(std::move(arguments)) {}
	~InstantiationExpr() override = default;
	void accept(ExpressionVisitor& visitor) override { return visitor.visitInstantiationExpr(*this); }

	const std::string& getName() const { return m_name; }
	const std::vector<Expression::Ptr>& getArguments() const { return m_arguments; }

private:
	std::string m_name;
	std::vector<Expression::Ptr> m_arguments;
};
}