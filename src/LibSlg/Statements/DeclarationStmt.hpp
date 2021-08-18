#pragma once

#include <string>
#include "Statement.hpp"
#include "../Token.hpp"

namespace LibSlg {
class DeclarationStmt : public Statement {
public:
	DeclarationStmt(Token identifier, Expression::Ptr initializer) :
			m_identifier(identifier), m_initializer(std::move(initializer)) {}
	void accept(StatementVisitor& visitor) override { visitor.visitDeclarationStmt(*this); }

	const Token& getIdentifier() const { return m_identifier; }
	const Expression::Ptr& getInitializer() const { return m_initializer; }
private:
	Token m_identifier;
	Expression::Ptr m_initializer;
};
}