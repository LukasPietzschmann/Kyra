#include "AccessExpr.hpp"

namespace Kyra {
AccessExpr::AccessExpr(const Position& position, const HasPtrAlias::Ptr& owner, Token name) :
	Expression(position), m_owner(owner), m_name(std::move(name)) {}

void AccessExpr::accept(ExpressionVisitor& visitor) { return visitor.visitAccessExpr(*this); }

Expression::Ptr AccessExpr::getOwner() const { return m_owner; }

const Token& AccessExpr::getName() const { return m_name; }
}