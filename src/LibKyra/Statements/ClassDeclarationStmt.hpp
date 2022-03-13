#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../Token.hpp"
#include "Statement.hpp"

namespace Kyra {
struct Position;
class VarDeclarationStmt;
class TypeExpr;
class ClassDeclarationStmt : public Statement {
public:
	struct ConstructorParameter {
		ConstructorParameter(Token name, bool is_mutable, std::shared_ptr<TypeExpr> type);
		Token name;
		bool is_mutable;
		std::shared_ptr<TypeExpr> type;
	};

	ClassDeclarationStmt(const Position& position,
			Token class_name,
			std::vector<ConstructorParameter>&& parameters,
			std::vector<std::shared_ptr<VarDeclarationStmt>>&& declarations);
	~ClassDeclarationStmt() override = default;

	void accept(StatementVisitor& visitor) override;
	const Token& get_class_name() const;
	const std::vector<ConstructorParameter>& get_constructor_parameters() const;
	const std::vector<std::shared_ptr<VarDeclarationStmt>>& get_declarations() const;

private:
	Token m_class_name;
	std::vector<ConstructorParameter> m_parameters;
	std::vector<std::shared_ptr<VarDeclarationStmt>> m_declarations;
};
}