#pragma once

#include "../Expressions/Expression.hpp"
#include "../Token.hpp"
#include "Statement.hpp"

namespace Kyra {
struct Position;
class DeclarationStmt : public Statement {
public:
	DeclarationStmt(const Position& position,
			Token identifier,
			const Expression::Ptr& initializer,
			const Expression::Ptr& type,
			bool isMutable = true);
	~DeclarationStmt() override = default;

	void accept(StatementVisitor& visitor) override;

	const Token& getIdentifier() const;
	Expression::Ptr getInitializer() const;
	Expression::Ptr getType() const;
	bool isMutable() const;

private:
	Token m_identifier;
	Expression::Ptr m_initializer;
	Expression::Ptr m_type;
	bool m_isMutable;
};
}