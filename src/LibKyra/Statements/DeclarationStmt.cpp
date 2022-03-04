#include "DeclarationStmt.hpp"

#include <utility>

namespace Kyra {
struct Position;
DeclarationStmt::DeclarationStmt(const Position& position,
		Token identifier,
		Expression::Ptr initializer,
		Expression::Ptr type,
		bool is_mutable) :
	Statement(position),
	m_identifier(std::move(identifier)), m_initializer(std::move(initializer)), m_type(std::move(type)),
	m_is_mutable(is_mutable) {}

void DeclarationStmt::accept(StatementVisitor& visitor) { visitor.visit_declaration_stmt(*this); }

const Token& DeclarationStmt::get_identifier() const { return m_identifier; }

Expression::Ptr DeclarationStmt::get_initializer() const { return m_initializer; }

Expression::Ptr DeclarationStmt::get_type() const { return m_type; }

bool DeclarationStmt::is_mutable() const { return m_is_mutable; }
}