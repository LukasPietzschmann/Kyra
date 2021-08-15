#pragma once

#include "Expression.hpp"
#include "../Statements/Statement.hpp"
#include "Variable.hpp"

namespace LibSlg {
class Function : public Expression {
public:
	Function(std::vector<Variable> parameters, Statement::Ptr impl) :
			m_parameters(std::move(parameters)), m_implementation(std::move(impl)) {}
	const std::vector<Variable>& getParameters() const { return m_parameters; }
	const Statement::Ptr& getImplementation() const { return m_implementation; }
private:
	std::vector<Variable> m_parameters;
	Statement::Ptr m_implementation;
};
}