#include "AstLogger.hpp"

#include <sstream>
#include <string>

#include "Expressions/AccessExpr.hpp"
#include "Expressions/AssignmentExpr.hpp"
#include "Expressions/BinaryExpr.hpp"
#include "Expressions/CallExpr.hpp"
#include "Expressions/FunctionExpr.hpp"
#include "Expressions/GroupExpr.hpp"
#include "Expressions/InstantiationExpr.hpp"
#include "Expressions/LiteralExpr.hpp"
#include "Expressions/TypeExpr.hpp"
#include "Expressions/UnaryExpr.hpp"
#include "Expressions/VariableExpr.hpp"
#include "Statements/BlockStmt.hpp"
#include "Statements/ClassDeclarationStmt.hpp"
#include "Statements/DeclarationStmt.hpp"
#include "Statements/ExpressionStmt.hpp"
#include "Statements/PrintStmt.hpp"
#include "Statements/ReturnStmt.hpp"
#include "Statements/WhileStmt.hpp"
#include "TokenType.hpp"

namespace Kyra {
void AstLogger::log_statement(const Statement::Ptr& statement) {
	m_indent = 0;
	statement->accept(*this);
}

void AstLogger::visit_block_stmt(BlockStmt& block_stmt) {
	COUT << "Block" << std::endl;
	++m_indent;
	for(const auto& stmt : block_stmt.get_statements())
		stmt->accept(*this);
	--m_indent;
}

void AstLogger::visit_declaration_stmt(DeclarationStmt& declaration_stmt) {
	COUT << "Declaration of variable " << declaration_stmt.get_identifier().get_value().as_string();
	if(declaration_stmt.get_initializer()) {
		std::cout << " to " << std::endl;
		++m_indent;
		declaration_stmt.get_initializer()->accept(*this);
		--m_indent;
	} else
		std::cout << std::endl;
}

void AstLogger::visit_class_declaration_stmt(ClassDeclarationStmt& class_declaration_stmt) {
	COUT << "Declaration of class " << class_declaration_stmt.get_identifier().get_value().as_string() << "\n";
	COUT << "Constructor parameter\n";
	++m_indent;
	std::stringstream parameter;
	for(const auto& param : class_declaration_stmt.get_constructor_parameters())
		parameter << param.name.get_value().as_string() << " ";
	if(!class_declaration_stmt.get_constructor_parameters().empty())
		COUT << parameter.str() << "\n";
	--m_indent;
	std::cout << "Declarations" << std::endl;
	++m_indent;
	for(const auto& decl : class_declaration_stmt.get_declarations())
		decl->accept(*this);
	--m_indent;
}

void AstLogger::visit_expression_stmt(ExpressionStmt& expression_stmt) {
	COUT << "Expression" << std::endl;
	++m_indent;
	expression_stmt.get_expr()->accept(*this);
	--m_indent;
}

void AstLogger::visit_print_stmt(PrintStmt& print_stmt) {
	COUT << "Print" << std::endl;
	++m_indent;
	print_stmt.get_expr()->accept(*this);
	--m_indent;
}

void AstLogger::visit_return_stmt(ReturnStmt& return_stmt) {
	COUT << "Return" << std::endl;
	++m_indent;
	return_stmt.get_expr()->accept(*this);
	--m_indent;
}

void AstLogger::visit_while_stmt(WhileStmt& while_stmt) {
	COUT << "While" << std::endl;
	++m_indent;
	while_stmt.get_condition()->accept(*this);
	--m_indent;
	COUT << "Do" << std::endl;
	++m_indent;
	while_stmt.get_statement()->accept(*this);
	--m_indent;
}

void AstLogger::visit_access_expr(AccessExpr& access_expr) {
	COUT << "Get Variable " << access_expr.get_name().get_value().as_string() << " from " << std::endl;
	++m_indent;
	access_expr.get_owner()->accept(*this);
	--m_indent;
}

void AstLogger::visit_assignment_expr(AssignmentExpr& assignment_expr) {
	COUT << "Assignment of variable " << assignment_expr.get_name().get_value().as_string() << " to " << std::endl;
	++m_indent;
	assignment_expr.get_new_value()->accept(*this);
	--m_indent;
}

void AstLogger::visit_binary_expr(BinaryExpr& binary_expr) {
	COUT << "Binary Expr with Operand " << TokenTypeName::get_for(binary_expr.get_operator().get_type()) << std::endl;
	COUT << "Lhs: " << std::endl;
	++m_indent;
	binary_expr.get_lhs()->accept(*this);
	--m_indent;
	COUT << "Rhs: " << std::endl;
	++m_indent;
	binary_expr.get_rhs()->accept(*this);
	--m_indent;
}

void AstLogger::visit_call_expr(CallExpr& call_expr) {
	COUT << "Call to" << std::endl;
	++m_indent;
	call_expr.get_function()->accept(*this);
	--m_indent;
	COUT << "With arguments" << std::endl;
	++m_indent;
	for(const auto& argument : call_expr.get_arguments())
		argument->accept(*this);
	--m_indent;
}

void AstLogger::visit_function(FunctionExpr& function_expr) {
	std::stringstream parameters;
	for(const auto& parameter : function_expr.get_parameters())
		parameters << parameter.name.get_value().as_string() << " ";
	COUT << "Function" << std::endl;
	COUT << "Parameter " << parameters.str() << std::endl;
	COUT << "Implementation" << std::endl;
	++m_indent;
	function_expr.get_implementation()->accept(*this);
	--m_indent;
}

void AstLogger::visit_group_expr(GroupExpr& group_expr) {
	COUT << "Group" << std::endl;
	++m_indent;
	group_expr.get_expr()->accept(*this);
	--m_indent;
}

void AstLogger::visit_instantiation_expr(InstantiationExpr& instantiation_expr) {
	COUT << "Instantiation of class " + instantiation_expr.get_name() + "\n";
	COUT << "With constructor arguments\n";
	++m_indent;
	for(const auto& argument : instantiation_expr.get_arguments())
		argument->accept(*this);
	--m_indent;
}

void AstLogger::visit_literal(LiteralExpr& literal) {
	COUT << "Literal with value of " << literal.get_value()->to_string() << std::endl;
}

void AstLogger::visit_type_expr(TypeExpr& type_expr) { COUT << "Type " << type_expr.get_name(); }

void AstLogger::visit_unary_expr(UnaryExpr& unary_expr) {
	COUT << "Unary Expr with Operand " << TokenTypeName::get_for(unary_expr.get_operator().get_type()) << std::endl;
	COUT << "Rhs: " << std::endl;
	++m_indent;
	unary_expr.get_rhs()->accept(*this);
	--m_indent;
}
void AstLogger::visit_variable(VariableExpr& variable_expr) {
	COUT << "Variable with identifier " << variable_expr.get_name().get_value().as_string() << std::endl;
}
}