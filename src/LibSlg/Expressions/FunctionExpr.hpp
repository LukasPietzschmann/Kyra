#pragma once

#include <vector>
#include "Expression.hpp"
#include "../Statements/Statement.hpp"
#include "../Token.hpp"

namespace LibSlg {
class FunctionExpr : public Expression {
public:
	typedef struct Parameter {
		Parameter(Token name, Value::Type type) : name(name), type(type) {}
		Token name;
		Value::Type type;
	} Parameter;
	FunctionExpr(std::vector<Parameter> parameters, Statement::Ptr impl) :
			m_parameters(std::move(parameters)), m_implementation(std::move(impl)) {}
	~FunctionExpr() override {}
	Value::Ptr accept(ExpressionVisitor& visitor) override { return visitor.visitFunction(*this); }
	const std::vector<Parameter>& getParameters() const { return m_parameters; }
	const Statement::Ptr& getImplementation() const { return m_implementation; }
private:
	std::vector<Parameter> m_parameters;
	Statement::Ptr m_implementation;
};
}