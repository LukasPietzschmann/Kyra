#include "AssignmentExpr.hpp"

namespace Kyra {
AssignmentExpr::AssignmentExpr(const Position& position,
		const Expression::Ptr& owner,
		Token name,
		const Expression::Ptr& newValue) :
	Expression(position),
	m_owner(owner), m_name(std::move(name)), m_newValue(newValue) {}

void AssignmentExpr::accept(ExpressionVisitor& visitor) { return visitor.visitAssignmentExpr(*this); }

Expression::Ptr AssignmentExpr::getOwner() const { return m_owner; }

const Token& AssignmentExpr::getName() const { return m_name; }

Expression::Ptr AssignmentExpr::getNewValue() const { return m_newValue; }
}