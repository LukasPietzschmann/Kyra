#pragma once

#include <vector>

#include "../Statements/DeclarationStmt.hpp"
#include "../Token.hpp"
#include "../Values/Value.hpp"
#include "Statement.hpp"

namespace LibSlg {
class ClassDeclarationStmt : public Statement {
public:
	typedef struct ConstructorParameter {
		ConstructorParameter(Token name, bool isMutable, Value::Type type) :
			name(name), isMutable(isMutable), type(type) {}
		Token name;
		bool isMutable;
		Value::Type type;
	} ConstructorParameter;

	ClassDeclarationStmt(const Token& identifier, std::vector<ConstructorParameter> parameters,
			std::vector<std::shared_ptr<DeclarationStmt>> declarations) :
		identifier(identifier),
		m_parameters(std::move(parameters)), m_declarations(std::move(declarations)) {}
	~ClassDeclarationStmt() override {}
	void accept(StatementVisitor& visitor) override { visitor.visitClassDeclarationStmt(*this); }
	const Token& getIdentifier() const { return identifier; }
	const std::vector<ConstructorParameter>& getConstructorParameters() const { return m_parameters; }
	const std::vector<std::shared_ptr<DeclarationStmt>>& getDeclarations() const { return m_declarations; }

private:
	Token identifier;
	std::vector<ConstructorParameter> m_parameters;
	std::vector<std::shared_ptr<DeclarationStmt>> m_declarations;
};
}