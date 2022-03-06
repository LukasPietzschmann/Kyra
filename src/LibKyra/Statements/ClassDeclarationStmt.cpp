#include "ClassDeclarationStmt.hpp"

#include <utility>

namespace Kyra {
struct Position;
class DeclarationStmt;
ClassDeclarationStmt::ConstructorParameter::ConstructorParameter(Token name, bool is_mutable, std::string type) :
	name(std::move(name)), is_mutable(is_mutable), type(std::move(type)) {}

ClassDeclarationStmt::ClassDeclarationStmt(const Position& position,
		Token identifier,
		std::vector<ConstructorParameter>&& parameters,
		std::vector<std::shared_ptr<DeclarationStmt>>&& declarations) :
	Statement(position),
	m_identifier(std::move(identifier)), m_parameters(std::move(parameters)), m_declarations(std::move(declarations)) {}

void ClassDeclarationStmt::accept(StatementVisitor& visitor) { visitor.visit_class_declaration_stmt(*this); }

const Token& ClassDeclarationStmt::get_identifier() const { return m_identifier; }

const std::vector<ClassDeclarationStmt::ConstructorParameter>& ClassDeclarationStmt::get_constructor_parameters()
		const {
	return m_parameters;
}

const std::vector<std::shared_ptr<DeclarationStmt>>& ClassDeclarationStmt::get_declarations() const {
	return m_declarations;
}
}