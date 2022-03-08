#pragma once

#include "../Expressions/Expression.hpp"
#include "../Token.hpp"
#include "Statement.hpp"

namespace Kyra {
struct Position;
class TypeExpr;
class DeclarationStmt : public Statement {
public:
	DeclarationStmt(const Position& position,
			Token identifier,
			Expression::Ptr initializer,
			std::shared_ptr<TypeExpr> type,
			bool is_mutable = true);
	~DeclarationStmt() override = default;

	void accept(StatementVisitor& visitor) override;

	const Token& get_identifier() const;
	Expression::Ptr get_initializer() const;
	std::shared_ptr<TypeExpr> get_type() const;
	bool is_mutable() const;

private:
	Token m_identifier;
	Expression::Ptr m_initializer;
	std::shared_ptr<TypeExpr> m_type;
	bool m_is_mutable;
};
}