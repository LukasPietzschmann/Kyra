#include "CallExpr.hpp"

namespace Kyra {
CallExpr::CallExpr(const Position& position, const Expression::Ptr& function, std::vector<Expression::Ptr> arguments) :
	Expression(position), m_function(function), m_arguments(std::move(arguments)) {}

void CallExpr::accept(ExpressionVisitor& visitor) { return visitor.visitCallExpr(*this); }

Expression::Ptr CallExpr::getFunction() const { return m_function; }

const std::vector<Expression::Ptr>& CallExpr::getArguments() const { return m_arguments; }
}