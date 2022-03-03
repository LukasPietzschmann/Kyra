#include "AssignmentExpr.hpp"

#include <utility>

namespace Kyra {
struct Position;
AssignmentExpr::AssignmentExpr(const Position& position,
		const Expression::Ptr& owner,
		Token name,
		const Expression::Ptr& new_value) :
	Expression(position),
	m_owner(owner), m_name(std::move(name)), m_new_value(new_value) {}

void AssignmentExpr::accept(ExpressionVisitor& visitor) { return visitor.visit_assignment_expr(*this); }

Expression::Ptr AssignmentExpr::get_owner() const { return m_owner; }

const Token& AssignmentExpr::get_name() const { return m_name; }

Expression::Ptr AssignmentExpr::get_new_value() const { return m_new_value; }
}