#pragma once

#include <string>
#include <utility>
#include "Statement.hpp"
#include "../Token.hpp"

namespace LibSlg {
class DeclarationStmt : public Statement {
public:
	DeclarationStmt(Token identifier, Expression::Ptr initializer, bool isMutable=true) :
			m_identifier(std::move(identifier)), m_initializer(std::move(initializer)), m_isMutable(isMutable) {}
	~DeclarationStmt() override {}
	void accept(StatementVisitor& visitor) override { visitor.visitDeclarationStmt(*this); }

	const Token& getIdentifier() const { return m_identifier; }
	const Expression::Ptr& getInitializer() const { return m_initializer; }
	bool isMutable() const { return m_isMutable; }
private:
	Token m_identifier;
	Expression::Ptr m_initializer;
	bool m_isMutable;
};
}