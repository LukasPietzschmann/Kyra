#include "FunctionExpr.hpp"

#include <utility>

namespace Kyra {
struct Position;
FunctionExpr::Parameter::Parameter(Token name, std::string type) :
	name(std::move(std::move(name))), type(std::move(std::move(type))) {}

FunctionExpr::FunctionExpr(const Position& position,
		std::vector<Parameter> parameters,
		Expression::Ptr return_type,
		Statement::Ptr impl) :
	Expression(position),
	m_parameters(std::move(parameters)), m_return_type(std::move(return_type)), m_implementation(std::move(impl)) {}

void FunctionExpr::accept(ExpressionVisitor& visitor) { return visitor.visit_function(*this); }

const std::vector<FunctionExpr::Parameter>& FunctionExpr::get_parameters() const { return m_parameters; }

Expression::Ptr FunctionExpr::get_return_type() const { return m_return_type; }

Statement::Ptr FunctionExpr::get_implementation() const { return m_implementation; }
}