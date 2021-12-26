#pragma once

#include <utility>
#include <vector>

#include "../Statements/Statement.hpp"
#include "../Token.hpp"
#include "Expression.hpp"

namespace Kyra {
class FunctionExpr : public Expression {
public:
	struct Parameter {
		Parameter(Token name, std::string type) : name(std::move(std::move(name))), type(std::move(std::move(type))) {}
		Token name;
		std::string type;
	};
	FunctionExpr(const Position& position,
			std::vector<Parameter> parameters,
			const Expression::Ptr& returnType,
			const Statement::Ptr& impl) :
		Expression(position),
		m_parameters(std::move(parameters)), m_returnType(returnType), m_implementation(impl) {}
	~FunctionExpr() override = default;
	void accept(ExpressionVisitor& visitor) override { return visitor.visitFunction(*this); }
	const std::vector<Parameter>& getParameters() const { return m_parameters; }
	Expression::Ptr getReturnType() const { return m_returnType; }
	Statement::Ptr getImplementation() const { return m_implementation; }

private:
	std::vector<Parameter> m_parameters;
	Expression::Ptr m_returnType;
	Statement::Ptr m_implementation;
};
}