#include "InstantiationExpr.hpp"

namespace Kyra {
InstantiationExpr::InstantiationExpr(const Position& position,
		std::string name,
		std::vector<Expression::Ptr> arguments) :
	Expression(position),
	m_name(std::move(name)), m_arguments(std::move(arguments)) {}

void InstantiationExpr::accept(ExpressionVisitor& visitor) { return visitor.visitInstantiationExpr(*this); }

const std::string& InstantiationExpr::getName() const { return m_name; }

const std::vector<Expression::Ptr>& InstantiationExpr::getArguments() const { return m_arguments; }
}