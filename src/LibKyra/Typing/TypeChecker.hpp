#pragma once

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "../Expressions/Expression.hpp"
#include "../Expressions/Forward.hpp"
#include "../Result.hpp"
#include "../Statements/Forward.hpp"
#include "../Statements/Statement.hpp"
#include "Type.hpp"
#include "TypeContext.hpp"
#include "TypingErrors.hpp"

namespace Kyra {
class FunctionType;
class TypeChecker : public ExpressionVisitor, public StatementVisitor {
#define THROW_TYPING_ERROR(error)                                         \
	do {                                                                  \
		m_result.insert_error({error.get_cause(), error.get_position()}); \
		return;                                                           \
	} while(0)

	EXPR_NEEDS_VISIT_RETURN_OF_TYPE(Type::Ptr);

public:
	static TypeChecker& the();
	TypeChecker(TypeChecker const&) = delete;
	TypeChecker(TypeChecker&&) = delete;
	void operator=(TypeChecker const&) = delete;
	void operator=(TypeChecker&&) = delete;

	const Result<EmptyValue>& check(const std::vector<Statement::Ptr>& statements);

	void visit_access_expr(AccessExpr& access_expr) override;
	void visit_assignment_expr(AssignmentExpr& assignment_expr) override;
	void visit_binary_expr(BinaryExpr& binary_expr) override;
	void visit_call_expr(CallExpr& call_expr) override;
	void visit_lambda_function(LambdaFunctionExpr& function_expr) override;
	void visit_group_expr(GroupExpr& group_expr) override;
	void visit_instantiation_expr(InstantiationExpr& instantiation_expr) override;
	void visit_literal(LiteralExpr& literal_expr) override;
	void visit_type_expr(TypeExpr& type_expr) override;
	void visit_unary_expr(UnaryExpr& unary_expr) override;
	void visit_variable(VariableExpr& variable_expr) override;

	void visit_block_stmt(BlockStmt& block_stmt) override;
	void visit_var_declaration_stmt(VarDeclarationStmt& declaration_stmt) override;
	void visit_class_declaration_stmt(ClassDeclarationStmt& class_declaration_stmt) override;
	void visit_fun_declaration_stmt(FunDeclarationStmt& fun_declaration_stmt) override;
	void visit_expression_stmt(ExpressionStmt& expression_stmt) override;
	void visit_print_stmt(PrintStmt& print_stmt) override;
	void visit_return_stmt(ReturnStmt& return_stmt) override;
	void visit_while_stmt(WhileStmt& while_stmt) override;
	void visit_if_stmt(IfStmt& if_stmt) override;

private:
	TypeChecker() = default;

	Result<EmptyValue> m_result;
	TypeContext::Ptr m_current_context{TypeContext::make_ptr<TypeContext>()};
	FunctionType* m_current_function{};
	bool m_does_current_function_return{};
	char* m_current_class_name{};

	void check(const Statement::Ptr& statement);
	template <class Callback>
	TypeContext::Ptr run_in_new_context(const Callback& function,
			TypeContext::Ptr parent,
			const TypeContext::Ptr& values_to_copy = nullptr);
};
}