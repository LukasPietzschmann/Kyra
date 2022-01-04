#include "ClassDeclarationStmt.hpp"

namespace Kyra {
ClassDeclarationStmt::ConstructorParameter::ConstructorParameter(Token name, bool isMutable, std::string type) :
	name(std::move(std::move(name))), isMutable(isMutable), type(std::move(std::move(type))) {}

ClassDeclarationStmt::ClassDeclarationStmt(const Position& position,
		Token identifier,
		std::vector<ConstructorParameter> parameters,
		std::vector<std::shared_ptr<DeclarationStmt>> declarations) :
	Statement(position),
	m_identifier(std::move(identifier)), m_parameters(std::move(parameters)), m_declarations(std::move(declarations)) {}

void ClassDeclarationStmt::accept(StatementVisitor& visitor) { visitor.visitClassDeclarationStmt(*this); }

const Token& ClassDeclarationStmt::getIdentifier() const { return m_identifier; }

const std::vector<ClassDeclarationStmt::ConstructorParameter>& ClassDeclarationStmt::getConstructorParameters() const {
	return m_parameters;
}

const std::vector<std::shared_ptr<DeclarationStmt>>& ClassDeclarationStmt::getDeclarations() const {
	return m_declarations;
}
}