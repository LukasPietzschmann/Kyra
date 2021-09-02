#pragma once

#include <string>
#include <utility>

#include "../Expressions/Expression.hpp"
#include "../Token.hpp"
#include "Statement.hpp"

namespace LibSlg {
class DeclarationStmt : public Statement {
public:
	DeclarationStmt(Token identifier, Expression::Ptr initializer, Value::Type type, bool isMutable = true) :
		m_identifier(std::move(identifier)), m_initializer(std::move(initializer)), m_type(std::move(type)),
		m_isMutable(isMutable) {}
	~DeclarationStmt() override {}
	void accept(StatementVisitor& visitor) override { visitor.visitDeclarationStmt(*this); }

	const Token& getIdentifier() const { return m_identifier; }
	const Expression::Ptr& getInitializer() const { return m_initializer; }
	const Value::Type& getType() const { return m_type; }
	bool isMutable() const { return m_isMutable; }

private:
	Token m_identifier;
	Expression::Ptr m_initializer;
	Value::Type m_type;
	bool m_isMutable;
};
}