#include "FunDeclarationStmt.hpp"

#include <utility>
#include <vector>

#include "../Expressions/LambdaFunctionExpr.hpp"
#include "../Expressions/TypeExpr.hpp"

namespace Kyra {

FunDeclarationStmt::FunDeclarationStmt(const Position& position,
		Token identifier,
		std::shared_ptr<LambdaFunctionExpr> function) :
	Statement(position),
	m_identifier(std::move(identifier)), m_function(std::move(function)) {
	std::vector<std::shared_ptr<TypeExpr>> param_types;
	for(const auto& param : m_function->get_parameters())
		param_types.push_back(param.type);
	// TODO: set accurate position
	m_type = std::make_shared<TypeExpr>(Position(0, 0, 0, 0), std::move(param_types), m_function->get_return_type());
}

void FunDeclarationStmt::accept(StatementVisitor& visitor) { visitor.visit_fun_declaration_stmt(*this); }

const Token& FunDeclarationStmt::get_identifier() const { return m_identifier; }

std::shared_ptr<LambdaFunctionExpr> FunDeclarationStmt::get_function() const { return m_function; }

std::shared_ptr<TypeExpr> FunDeclarationStmt::get_type() const { return m_type; }
}