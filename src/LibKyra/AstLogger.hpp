#pragma once

#include <iostream>

#include "Expressions/Expression.hpp"
#include "Expressions/Forward.hpp"
#include "Statements/Forward.hpp"
#include "Statements/Statement.hpp"

#define COUT std::cout << std::string(m_indent * 4, ' ')

namespace Kyra {
class AstLogger : public StatementVisitor, public ExpressionVisitor {
public:
	AstLogger() = default;
	~AstLogger() override = default;

	void log_statement(const Statement::Ptr& statement);

	void visit_block_stmt(BlockStmt& block_stmt) override;
	void visit_declaration_stmt(DeclarationStmt& declaration_stmt) override;
	void visit_class_declaration_stmt(ClassDeclarationStmt& class_declaration_stmt) override;
	void visit_expression_stmt(ExpressionStmt& expression_stmt) override;
	void visit_print_stmt(PrintStmt& print_stmt) override;
	void visit_return_stmt(ReturnStmt& return_stmt) override;
	void visit_while_stmt(WhileStmt& while_stmt) override;
	void visit_if_stmt(IfStmt& if_stmt) override;
	void visit_access_expr(AccessExpr& access_expr) override;
	void visit_assignment_expr(AssignmentExpr& assignment_expr) override;
	void visit_binary_expr(BinaryExpr& binary_expr) override;
	void visit_call_expr(CallExpr& call_expr) override;
	void visit_function(FunctionExpr& function_expr) override;
	void visit_group_expr(GroupExpr& group_expr) override;
	void visit_instantiation_expr(InstantiationExpr& instantiation_expr) override;
	void visit_literal(LiteralExpr& literal) override;
	void visit_type_expr(TypeExpr& type_expr) override;
	void visit_unary_expr(UnaryExpr& unary_expr) override;
	void visit_variable(VariableExpr& variable_expr) override;

private:
	unsigned int m_indent{};
};
}