#include "AccessExpr.hpp"

#include <utility>

namespace Kyra {
struct Position;
AccessExpr::AccessExpr(const Position& position, const HasPtrAlias::Ptr& owner, Token name) :
	Expression(position), m_owner(owner), m_name(std::move(name)) {}

void AccessExpr::accept(ExpressionVisitor& visitor) { return visitor.visit_access_expr(*this); }

Expression::Ptr AccessExpr::get_owner() const { return m_owner; }

const Token& AccessExpr::get_name() const { return m_name; }
}