#include "CallExpr.hpp"

namespace Kyra {
CallExpr::CallExpr(const Position& position, const Expression::Ptr& function, std::vector<Expression::Ptr> arguments) :
	Expression(position), m_function(function), m_arguments(std::move(arguments)) {}

void CallExpr::accept(ExpressionVisitor& visitor) { return visitor.visit_call_expr(*this); }

Expression::Ptr CallExpr::get_function() const { return m_function; }

const std::vector<Expression::Ptr>& CallExpr::get_arguments() const { return m_arguments; }
}