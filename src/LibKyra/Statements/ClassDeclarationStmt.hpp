#pragma once

#include <utility>
#include <vector>

#include "../Statements/DeclarationStmt.hpp"
#include "../Token.hpp"
#include "../Values/Value.hpp"
#include "Statement.hpp"

namespace Kyra {
class ClassDeclarationStmt : public Statement {
public:
	struct ConstructorParameter {
		ConstructorParameter(Token name, bool isMutable, std::string type);
		Token name;
		bool isMutable;
		std::string type;
	};

	ClassDeclarationStmt(const Position& position,
			Token identifier,
			std::vector<ConstructorParameter> parameters,
			std::vector<std::shared_ptr<DeclarationStmt>> declarations);
	~ClassDeclarationStmt() override = default;

	void accept(StatementVisitor& visitor) override;
	const Token& getIdentifier() const;
	const std::vector<ConstructorParameter>& getConstructorParameters() const;
	const std::vector<std::shared_ptr<DeclarationStmt>>& getDeclarations() const;

private:
	Token m_identifier;
	std::vector<ConstructorParameter> m_parameters;
	std::vector<std::shared_ptr<DeclarationStmt>> m_declarations;
};
}