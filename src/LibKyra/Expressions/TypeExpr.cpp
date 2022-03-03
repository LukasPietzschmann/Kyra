#include "TypeExpr.hpp"

#include <cassert>
#include <utility>

namespace Kyra {
struct Position;
TypeExpr::TypesForFunctions::TypesForFunctions(std::vector<Expression::Ptr> parameter_types,
		Expression::Ptr return_type) :
	parameter_types(std::move(parameter_types)),
	return_type(std::move(return_type)) {}

TypeExpr::TypeExpr(const Position& position, std::string name) :
	Expression(position), m_name(std::move(name)), m_types_for_functions(nullptr) {}

TypeExpr::TypeExpr(const Position& position,
		std::vector<Expression::Ptr> parameter_types,
		Expression::Ptr return_type) :
	Expression(position),
	m_name("Function"), m_types_for_functions(std::make_shared<TypesForFunctions>(parameter_types, return_type)) {}

void TypeExpr::accept(ExpressionVisitor& visitor) { visitor.visit_type_expr(*this); }

const std::string& TypeExpr::get_name() const { return m_name; }

const std::vector<Expression::Ptr>& TypeExpr::get_parameter_types() const {
	assert(m_types_for_functions != nullptr);
	return m_types_for_functions->parameter_types;
}

const Expression::Ptr& TypeExpr::get_return_type() const {
	assert(m_types_for_functions != nullptr);
	return m_types_for_functions->return_type;
}

bool TypeExpr::is_function() const { return m_types_for_functions != nullptr; }
}