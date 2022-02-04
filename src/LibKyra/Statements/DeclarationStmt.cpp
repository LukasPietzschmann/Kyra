#include "DeclarationStmt.hpp"

#include <utility>

namespace Kyra {
struct Position;
DeclarationStmt::DeclarationStmt(const Position& position,
		Token identifier,
		const Expression::Ptr& initializer,
		const Expression::Ptr& type,
		bool isMutable) :
	Statement(position),
	m_identifier(std::move(identifier)), m_initializer(initializer), m_type(type), m_isMutable(isMutable) {}

void DeclarationStmt::accept(StatementVisitor& visitor) { visitor.visitDeclarationStmt(*this); }

const Token& DeclarationStmt::getIdentifier() const { return m_identifier; }

Expression::Ptr DeclarationStmt::getInitializer() const { return m_initializer; }

Expression::Ptr DeclarationStmt::getType() const { return m_type; }

bool DeclarationStmt::isMutable() const { return m_isMutable; }
}