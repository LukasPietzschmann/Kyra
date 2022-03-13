#include "VarDeclarationStmt.hpp"

#include <utility>

namespace Kyra {
struct Position;
VarDeclarationStmt::VarDeclarationStmt(const Position& position,
		Token identifier,
		Expression::Ptr initializer,
		std::shared_ptr<TypeExpr> type,
		bool is_mutable) :
	Statement(position),
	m_identifier(std::move(identifier)), m_initializer(std::move(initializer)), m_type(std::move(type)),
	m_is_mutable(is_mutable) {}

void VarDeclarationStmt::accept(StatementVisitor& visitor) { visitor.visit_var_declaration_stmt(*this); }

const Token& VarDeclarationStmt::get_identifier() const { return m_identifier; }

Expression::Ptr VarDeclarationStmt::get_initializer() const { return m_initializer; }

std::shared_ptr<TypeExpr> VarDeclarationStmt::get_type() const { return m_type; }

bool VarDeclarationStmt::is_mutable() const { return m_is_mutable; }
}