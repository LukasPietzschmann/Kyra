#include "FunctionExpr.hpp"

#include <utility>

namespace Kyra {
struct Position;
FunctionExpr::Parameter::Parameter(Token name, std::string type) :
	name(std::move(std::move(name))), type(std::move(std::move(type))) {}

FunctionExpr::FunctionExpr(const Position& position,
		std::vector<Parameter> parameters,
		const Expression::Ptr& returnType,
		const Statement::Ptr& impl) :
	Expression(position),
	m_parameters(std::move(parameters)), m_returnType(returnType), m_implementation(impl) {}

void FunctionExpr::accept(ExpressionVisitor& visitor) { return visitor.visitFunction(*this); }

const std::vector<FunctionExpr::Parameter>& FunctionExpr::getParameters() const { return m_parameters; }

Expression::Ptr FunctionExpr::getReturnType() const { return m_returnType; }

Statement::Ptr FunctionExpr::getImplementation() const { return m_implementation; }
}