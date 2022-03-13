#include "LambdaFunctionExpr.hpp"

#include <utility>

namespace Kyra {
struct Position;
LambdaFunctionExpr::Parameter::Parameter(Token name, std::shared_ptr<TypeExpr> type) :
	name(std::move(name)), type(std::move(type)) {}

LambdaFunctionExpr::LambdaFunctionExpr(const Position& position,
		std::vector<Parameter>&& parameters,
		std::shared_ptr<TypeExpr> return_type,
		Statement::Ptr impl) :
	Expression(position),
	m_parameters(std::move(parameters)), m_return_type(std::move(return_type)), m_implementation(std::move(impl)) {}

void LambdaFunctionExpr::accept(ExpressionVisitor& visitor) { return visitor.visit_lambda_function(*this); }

const std::vector<LambdaFunctionExpr::Parameter>& LambdaFunctionExpr::get_parameters() const { return m_parameters; }

std::shared_ptr<TypeExpr> LambdaFunctionExpr::get_return_type() const { return m_return_type; }

Statement::Ptr LambdaFunctionExpr::get_implementation() const { return m_implementation; }
}