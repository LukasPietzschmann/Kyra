#include "InstantiationExpr.hpp"

#include <utility>

namespace Kyra {
struct Position;
InstantiationExpr::InstantiationExpr(const Position& position,
		std::string name,
		std::vector<Expression::Ptr> arguments) :
	Expression(position),
	m_name(std::move(name)), m_arguments(std::move(arguments)) {}

void InstantiationExpr::accept(ExpressionVisitor& visitor) { return visitor.visit_instantiation_expr(*this); }

const std::string& InstantiationExpr::get_name() const { return m_name; }

const std::vector<Expression::Ptr>& InstantiationExpr::get_arguments() const { return m_arguments; }
}