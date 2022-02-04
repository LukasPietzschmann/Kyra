#include "TypeExpr.hpp"

#include <cassert>
#include <utility>

namespace Kyra {
struct Position;
TypeExpr::TypesForFunctions::TypesForFunctions(std::vector<Expression::Ptr> parameterTypes,
		Expression::Ptr returnType) :
	parameterTypes(std::move(parameterTypes)),
	returnType(std::move(returnType)) {}

TypeExpr::TypeExpr(const Position& position, std::string name) :
	Expression(position), m_name(std::move(name)), m_typesForFunctions(nullptr) {}

TypeExpr::TypeExpr(const Position& position, std::vector<Expression::Ptr> parameterTypes, Expression::Ptr returnType) :
	Expression(position), m_name("Function"),
	m_typesForFunctions(std::make_shared<TypesForFunctions>(parameterTypes, returnType)) {}

void TypeExpr::accept(ExpressionVisitor& visitor) { visitor.visitTypeExpr(*this); }

const std::string& TypeExpr::getName() const { return m_name; }

const std::vector<Expression::Ptr>& TypeExpr::getParameterTypes() const {
	assert(m_typesForFunctions != nullptr);
	return m_typesForFunctions->parameterTypes;
}

const Expression::Ptr& TypeExpr::getReturnType() const {
	assert(m_typesForFunctions != nullptr);
	return m_typesForFunctions->returnType;
}

bool TypeExpr::isFunction() const { return m_typesForFunctions != nullptr; }
}