#pragma once

#include "../Expressions/Expression.hpp"
#include "../Statements/Statement.hpp"
#include "../Values/Value.hpp"
#include "RuntimeContext.hpp"

namespace Kyra {
class AccessExpr;
class AssignmentExpr;
class BinaryExpr;
class BlockStmt;
class CallExpr;
class ClassDeclarationStmt;
class DeclarationStmt;
class ExpressionStmt;
class FunctionExpr;
class GroupExpr;
class InstantiationExpr;
class LiteralExpr;
class PrintStmt;
class ReturnStmt;
class TypeExpr;
class UnaryExpr;
class VariableExpr;
class WhileStmt;

class Runtime : public ExpressionVisitor, public StatementVisitor {
	EXPR_NEEDS_VISIT_RETURN_OF_TYPE(Value::Ptr);

public:
	static Runtime& the();

	Runtime(Runtime const&) = delete;
	Runtime(Runtime&&) noexcept = delete;
	Runtime& operator=(Runtime const&) = delete;
	Runtime& operator=(Runtime&&) = delete;

	void execute_statement(Statement::Ptr statement, RuntimeContext::Ptr context_to_execute_on = nullptr);
	Value::Ptr execute_expression(Expression::Ptr expression, RuntimeContext::Ptr context_to_execute_on = nullptr);

	void visit_access_expr(AccessExpr& access_expr) override;
	void visit_assignment_expr(AssignmentExpr& assignment_expr) override;
	void visit_binary_expr(BinaryExpr& binary_expr) override;
	void visit_call_expr(CallExpr& call_expr) override;
	void visit_function(FunctionExpr& function_expr) override;
	void visit_group_expr(GroupExpr& group_expr) override;
	void visit_instantiation_expr(InstantiationExpr& instantiation_expr) override;
	void visit_literal(LiteralExpr& literal_expr) override;
	void visit_type_expr(TypeExpr& type_expr) override;
	void visit_unary_expr(UnaryExpr& unary_expr) override;
	void visit_variable(VariableExpr& variable_expr) override;

	void visit_block_stmt(BlockStmt& block_stmt) override;
	void visit_declaration_stmt(DeclarationStmt& declaration_stmt) override;
	void visit_class_declaration_stmt(ClassDeclarationStmt& class_declaration_stmt) override;
	void visit_expression_stmt(ExpressionStmt& expression_stmt) override;
	void visit_print_stmt(PrintStmt& print_stmt) override;
	void visit_return_stmt(ReturnStmt& return_stmt) override;
	void visit_while_stmt(WhileStmt& while_stmt) override;

private:
	Runtime() = default;

	RuntimeContext::Ptr m_current_context{RuntimeContext::make_ptr<RuntimeContext>()};

	template <typename Callback>
	RuntimeContext::Ptr run_in_new_context(const Callback& callback, RuntimeContext::Ptr parent);
};
}