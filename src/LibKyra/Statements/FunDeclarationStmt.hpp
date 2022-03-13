#pragma once

#include <memory>

#include "../Position.hpp"
#include "../Token.hpp"
#include "Statement.hpp"

namespace Kyra {
class LambdaFunctionExpr;
class TypeExpr;
class FunDeclarationStmt : public Statement {
public:
	FunDeclarationStmt(const Position& position, Token identifier, std::shared_ptr<LambdaFunctionExpr> function);
	~FunDeclarationStmt() override = default;

	void accept(StatementVisitor& visitor) override;

	const Token& get_identifier() const;
	std::shared_ptr<LambdaFunctionExpr> get_function() const;
	std::shared_ptr<TypeExpr> get_type() const;

private:
	Token m_identifier;
	std::shared_ptr<LambdaFunctionExpr> m_function;
	std::shared_ptr<TypeExpr> m_type;
};
}