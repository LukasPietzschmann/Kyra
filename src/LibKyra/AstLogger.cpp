#include "AstLogger.hpp"

#include <sstream>
#include <string>

#include "Expressions/AccessExpr.hpp"
#include "Expressions/AssignmentExpr.hpp"
#include "Expressions/BinaryExpr.hpp"
#include "Expressions/CallExpr.hpp"
#include "Expressions/GroupExpr.hpp"
#include "Expressions/InstantiationExpr.hpp"
#include "Expressions/LambdaFunctionExpr.hpp"
#include "Expressions/LiteralExpr.hpp"
#include "Expressions/TypeExpr.hpp"
#include "Expressions/UnaryExpr.hpp"
#include "Expressions/VariableExpr.hpp"
#include "Statements/BlockStmt.hpp"
#include "Statements/ClassDeclarationStmt.hpp"
#include "Statements/ExpressionStmt.hpp"
#include "Statements/FunDeclarationStmt.hpp"
#include "Statements/IfStmt.hpp"
#include "Statements/PrintStmt.hpp"
#include "Statements/ReturnStmt.hpp"
#include "Statements/VarDeclarationStmt.hpp"
#include "Statements/WhileStmt.hpp"
#include "TokenType.hpp"

namespace Kyra {
void AstLogger::log_statement(const Statement::Ptr& statement) {
	m_indent = 0;
	statement->accept(*this);
	std::flush(std::cout);
}

void AstLogger::visit_block_stmt(BlockStmt& block_stmt) {
	COUT << "Block\n";
	++m_indent;
	for(const auto& stmt : block_stmt.get_statements())
		stmt->accept(*this);
	--m_indent;
}

void AstLogger::visit_var_declaration_stmt(VarDeclarationStmt& declaration_stmt) {
	COUT << "Declaration of variable " << declaration_stmt.get_identifier().get_value().as_string();
	if(declaration_stmt.get_initializer()) {
		std::cout << " to\n";
		++m_indent;
		declaration_stmt.get_initializer()->accept(*this);
		--m_indent;
	} else
		std::cout << '\n';
}

void AstLogger::visit_class_declaration_stmt(ClassDeclarationStmt& class_declaration_stmt) {
	COUT << "Declaration of class " << class_declaration_stmt.get_class_name().get_value().as_string() << '\n';
	if(!class_declaration_stmt.get_constructor_parameters().empty()) {
		COUT << "Constructor parameter\n";
		++m_indent;
		std::stringstream parameter;
		for(const auto& param : class_declaration_stmt.get_constructor_parameters())
			parameter << param.name.get_value().as_string() << " ";
		if(!class_declaration_stmt.get_constructor_parameters().empty())
			COUT << parameter.str() << "\n";
		--m_indent;
	}
	if(!class_declaration_stmt.get_declarations().empty()) {
		std::cout << "Declarations\n";
		++m_indent;
		for(const auto& decl : class_declaration_stmt.get_declarations())
			decl->accept(*this);
		--m_indent;
	}
}

void AstLogger::visit_fun_declaration_stmt(FunDeclarationStmt& fun_declaration_stmt) {
	COUT << "Declaration of function " << fun_declaration_stmt.get_identifier().get_value().as_string();
	if(!fun_declaration_stmt.get_function()->get_parameters().empty()) {
		COUT << "Parameters\n";
		++m_indent;
		for(const auto& parameter : fun_declaration_stmt.get_function()->get_parameters())
			COUT << parameter.name.get_value().as_string() << '\n';
		--m_indent;
	}
	COUT << "Implementation\n";
	++m_indent;
	fun_declaration_stmt.get_function()->get_implementation()->accept(*this);
	--m_indent;
}

void AstLogger::visit_expression_stmt(ExpressionStmt& expression_stmt) {
	COUT << "Expression\n";
	++m_indent;
	expression_stmt.get_expr()->accept(*this);
	--m_indent;
}

void AstLogger::visit_print_stmt(PrintStmt& print_stmt) {
	COUT << "Print\n";
	++m_indent;
	print_stmt.get_expr()->accept(*this);
	--m_indent;
}

void AstLogger::visit_return_stmt(ReturnStmt& return_stmt) {
	COUT << "Return\n";
	++m_indent;
	return_stmt.get_expr()->accept(*this);
	--m_indent;
}

void AstLogger::visit_while_stmt(WhileStmt& while_stmt) {
	COUT << "While\n";
	++m_indent;
	while_stmt.get_condition()->accept(*this);
	--m_indent;
	COUT << "Do\n";
	++m_indent;
	while_stmt.get_statement()->accept(*this);
	--m_indent;
}

void AstLogger::visit_if_stmt(IfStmt& if_stmt) {
	COUT << "If\n";
	++m_indent;
	if_stmt.get_condition()->accept(*this);
	--m_indent;
	COUT << "Then\n";
	++m_indent;
	if_stmt.get_then()->accept(*this);
	--m_indent;
	if(if_stmt.get_else() != nullptr) {
		COUT << "Else\n";
		++m_indent;
		if_stmt.get_else()->accept(*this);
		--m_indent;
	}
}

void AstLogger::visit_access_expr(AccessExpr& access_expr) {
	COUT << "Get variable " << access_expr.get_name().get_value().as_string();
	if(access_expr.get_owner() != nullptr) {
		std::cout << " from\n";
		++m_indent;
		access_expr.get_owner()->accept(*this);
		--m_indent;
	} else
		std::cout << '\n';
}

void AstLogger::visit_assignment_expr(AssignmentExpr& assignment_expr) {
	COUT << "Assignment of variable " << assignment_expr.get_name().get_value().as_string() << " to\n";
	++m_indent;
	assignment_expr.get_new_value()->accept(*this);
	--m_indent;
}

void AstLogger::visit_binary_expr(BinaryExpr& binary_expr) {
	COUT << "Binary expr with operand " << TokenTypeName::get_for(binary_expr.get_operator().get_type()) << '\n';
	COUT << "Lhs\n";
	++m_indent;
	binary_expr.get_lhs()->accept(*this);
	--m_indent;
	COUT << "Rhs\n";
	++m_indent;
	binary_expr.get_rhs()->accept(*this);
	--m_indent;
}

void AstLogger::visit_call_expr(CallExpr& call_expr) {
	COUT << "Call to\n";
	++m_indent;
	call_expr.get_function()->accept(*this);
	--m_indent;
	if(!call_expr.get_arguments().empty()) {
		COUT << "With arguments\n";
		++m_indent;
		for(const auto& argument : call_expr.get_arguments())
			argument->accept(*this);
		--m_indent;
	}
}

void AstLogger::visit_lambda_function(LambdaFunctionExpr& function_expr) {
	COUT << "Function\n";
	if(!function_expr.get_parameters().empty()) {
		COUT << "Parameters\n";
		++m_indent;
		for(const auto& parameter : function_expr.get_parameters())
			COUT << parameter.name.get_value().as_string() << '\n';
		--m_indent;
	}
	COUT << "Implementation\n";
	++m_indent;
	function_expr.get_implementation()->accept(*this);
	--m_indent;
}

void AstLogger::visit_group_expr(GroupExpr& group_expr) {
	COUT << "Group\n";
	++m_indent;
	group_expr.get_expr()->accept(*this);
	--m_indent;
}

void AstLogger::visit_instantiation_expr(InstantiationExpr& instantiation_expr) {
	COUT << "Instantiation of class " + instantiation_expr.get_class_name().get_value().as_string() + '\n';
	if(!instantiation_expr.get_arguments().empty()) {
		COUT << "With constructor arguments\n";
		++m_indent;
		for(const auto& argument : instantiation_expr.get_arguments())
			argument->accept(*this);
		--m_indent;
	}
}

void AstLogger::visit_literal(LiteralExpr& literal) {
	switch(literal.get_type()) {
		case LiteralExpr::t_int: COUT << "Int-Literal with value of " << literal.get_int_value() << '\n'; break;
		case LiteralExpr::t_string:
			COUT << "String-Literal with value of " << literal.get_string_value() << '\n';
			break;
		case LiteralExpr::t_bool:
			COUT << "Bool-Literal with value of " << (literal.get_bool_value() ? "true" : "false") << '\n';
			break;
		case LiteralExpr::t_nothing: COUT << "Nothing-Literal with value of Nothing" << '\n'; break;
	}
}

void AstLogger::visit_type_expr(TypeExpr& type_expr) { COUT << "Type " << type_expr.get_name(); }

void AstLogger::visit_unary_expr(UnaryExpr& unary_expr) {
	COUT << "Unary expr with operand " << TokenTypeName::get_for(unary_expr.get_operator().get_type()) << '\n';
	COUT << "Rhs\n";
	++m_indent;
	unary_expr.get_rhs()->accept(*this);
	--m_indent;
}
void AstLogger::visit_variable(VariableExpr& variable_expr) {
	COUT << "Variable with identifier " << variable_expr.get_name().get_value().as_string() << '\n';
}
}