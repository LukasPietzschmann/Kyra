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
			bool is_mutable = true);
	~DeclarationStmt() override = default;

	void accept(StatementVisitor& visitor) override;

	const Token& get_identifier() const;
	Expression::Ptr get_initializer() const;
	Expression::Ptr get_type() const;
	bool is_mutable() const;

private:
	Token m_identifier;
	Expression::Ptr m_initializer;
	Expression::Ptr m_type;
	bool m_is_mutable;
};
}