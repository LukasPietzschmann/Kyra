#pragma once

#include <utility>
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
			name(std::move(std::move(name))), isMutable(isMutable), type(std::move(std::move(type))) {}
		Token name;
		bool isMutable;
		Value::Type type;
	} ConstructorParameter;

	ClassDeclarationStmt(const Position& position,
			Token identifier,
			std::vector<ConstructorParameter> parameters,
			std::vector<std::shared_ptr<DeclarationStmt>> declarations) :
		Statement(position),
		m_identifier(std::move(identifier)), m_parameters(std::move(parameters)),
		m_declarations(std::move(declarations)) {}
	~ClassDeclarationStmt() override = default;
	void accept(StatementVisitor& visitor) override { visitor.visitClassDeclarationStmt(*this); }
	const Token& getIdentifier() const { return m_identifier; }
	const std::vector<ConstructorParameter>& getConstructorParameters() const { return m_parameters; }
	const std::vector<std::shared_ptr<DeclarationStmt>>& getDeclarations() const { return m_declarations; }

private:
	Token m_identifier;
	std::vector<ConstructorParameter> m_parameters;
	std::vector<std::shared_ptr<DeclarationStmt>> m_declarations;
};
}