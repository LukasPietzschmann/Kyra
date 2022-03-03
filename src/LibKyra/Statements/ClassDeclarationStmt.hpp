#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../Token.hpp"
#include "Statement.hpp"

namespace Kyra {
struct Position;
class DeclarationStmt;
class ClassDeclarationStmt : public Statement {
public:
	struct ConstructorParameter {
		ConstructorParameter(Token name, bool is_mutable, std::string type);
		Token name;
		bool is_mutable;
		std::string type;
	};

	ClassDeclarationStmt(const Position& position,
			Token identifier,
			std::vector<ConstructorParameter> parameters,
			std::vector<std::shared_ptr<DeclarationStmt>> declarations);
	~ClassDeclarationStmt() override = default;

	void accept(StatementVisitor& visitor) override;
	const Token& get_identifier() const;
	const std::vector<ConstructorParameter>& get_constructor_parameters() const;
	const std::vector<std::shared_ptr<DeclarationStmt>>& get_declarations() const;

private:
	Token m_identifier;
	std::vector<ConstructorParameter> m_parameters;
	std::vector<std::shared_ptr<DeclarationStmt>> m_declarations;
};
}