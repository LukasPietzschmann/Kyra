#pragma once

#include <string>
#include <utility>

#include "../Expressions/Expression.hpp"
#include "../Token.hpp"
#include "Statement.hpp"

namespace Slanguage {
class DeclarationStmt : public Statement {
public:
	DeclarationStmt(const Position& position,
			Token identifier,
			const Expression::Ptr& initializer,
			const Expression::Ptr& type,
			bool isMutable = true) :
		Statement(position),
		m_identifier(std::move(identifier)), m_initializer(initializer), m_type(type), m_isMutable(isMutable) {}
	~DeclarationStmt() override = default;
	void accept(StatementVisitor& visitor) override { visitor.visitDeclarationStmt(*this); }

	const Token& getIdentifier() const { return m_identifier; }
	Expression::Ptr getInitializer() const { return m_initializer; }
	Expression::Ptr getType() const { return m_type; }
	bool isMutable() const { return m_isMutable; }

private:
	Token m_identifier;
	Expression::Ptr m_initializer;
	Expression::Ptr m_type;
	bool m_isMutable;
};
}