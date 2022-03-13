#include "ClassDeclarationStmt.hpp"

#include <utility>

namespace Kyra {
struct Position;
class VarDeclarationStmt;
ClassDeclarationStmt::ConstructorParameter::ConstructorParameter(Token name,
		bool is_mutable,
		std::shared_ptr<TypeExpr> type) :
	name(std::move(name)),
	is_mutable(is_mutable), type(std::move(type)) {}

ClassDeclarationStmt::ClassDeclarationStmt(const Position& position,
		Token class_name,
		std::vector<ConstructorParameter>&& parameters,
		std::vector<std::shared_ptr<VarDeclarationStmt>>&& declarations) :
	Statement(position),
	m_class_name(std::move(class_name)), m_parameters(std::move(parameters)), m_declarations(std::move(declarations)) {}

void ClassDeclarationStmt::accept(StatementVisitor& visitor) { visitor.visit_class_declaration_stmt(*this); }

const Token& ClassDeclarationStmt::get_class_name() const { return m_class_name; }

const std::vector<ClassDeclarationStmt::ConstructorParameter>& ClassDeclarationStmt::get_constructor_parameters()
		const {
	return m_parameters;
}

const std::vector<std::shared_ptr<VarDeclarationStmt>>& ClassDeclarationStmt::get_declarations() const {
	return m_declarations;
}
}