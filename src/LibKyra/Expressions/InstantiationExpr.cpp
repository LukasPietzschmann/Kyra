#include "InstantiationExpr.hpp"

#include <utility>

namespace Kyra {
struct Position;
InstantiationExpr::InstantiationExpr(const Position& position, Token name, std::vector<Expression::Ptr>&& arguments) :
	Expression(position), m_class_name(std::move(name)), m_arguments(std::move(arguments)) {}

void InstantiationExpr::accept(ExpressionVisitor& visitor) { return visitor.visit_instantiation_expr(*this); }

const Token& InstantiationExpr::get_class_name() const { return m_class_name; }

const std::vector<Expression::Ptr>& InstantiationExpr::get_arguments() const { return m_arguments; }
}