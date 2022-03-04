#include "AssignmentExpr.hpp"

#include <utility>

namespace Kyra {
struct Position;
AssignmentExpr::AssignmentExpr(const Position& position, Expression::Ptr owner, Token name, Expression::Ptr new_value) :
	Expression(position), m_owner(std::move(owner)), m_name(std::move(name)), m_new_value(std::move(new_value)) {}

void AssignmentExpr::accept(ExpressionVisitor& visitor) { return visitor.visit_assignment_expr(*this); }

Expression::Ptr AssignmentExpr::get_owner() const { return m_owner; }

const Token& AssignmentExpr::get_name() const { return m_name; }

Expression::Ptr AssignmentExpr::get_new_value() const { return m_new_value; }
}