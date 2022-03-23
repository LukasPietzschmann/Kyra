#include "Runtime.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../Expressions/AccessExpr.hpp"
#include "../Expressions/AssignmentExpr.hpp"
#include "../Expressions/BinaryExpr.hpp"
#include "../Expressions/CallExpr.hpp"
#include "../Expressions/GroupExpr.hpp"
#include "../Expressions/InstantiationExpr.hpp"
#include "../Expressions/LiteralExpr.hpp"
#include "../Expressions/UnaryExpr.hpp"
#include "../Expressions/VariableExpr.hpp"
#include "../Statements/BlockStmt.hpp"
#include "../Statements/ClassDeclarationStmt.hpp"
#include "../Statements/ExpressionStmt.hpp"
#include "../Statements/FunDeclarationStmt.hpp"
#include "../Statements/IfStmt.hpp"
#include "../Statements/PrintStmt.hpp"
#include "../Statements/ReturnStmt.hpp"
#include "../Statements/VarDeclarationStmt.hpp"
#include "../Statements/WhileStmt.hpp"
#include "../Values/Bool.hpp"
#include "../Values/Function.hpp"
#include "../Values/Klass.hpp"
#include "../Values/Nothing.hpp"
#include "../Values/Number.hpp"
#include "../Values/String.hpp"

namespace Kyra {
class LambdaFunctionExpr;
class TypeExpr;
Runtime& Runtime::the() {
	static Runtime instance;
	return instance;
}

void Runtime::execute_statement(const Statement::Ptr& statement, const RuntimeContext::Ptr& context_to_execute_on) {
	if(context_to_execute_on == nullptr) {
		statement->accept(*this);
		return;
	}

	RuntimeContext::Ptr context_copy = std::move(m_current_context);
	m_current_context = context_to_execute_on;
	try {
		statement->accept(*this);
	} catch(const ReturnException&) {
		m_current_context = std::move(context_copy);
		throw;
	}
	m_current_context = std::move(context_copy);
}

Value::Ptr Runtime::execute_expression(const Expression::Ptr& expression,
		const RuntimeContext::Ptr& context_to_execute_on) {
	if(context_to_execute_on == nullptr) {
		EXPR_ACCEPT(expression, *this, Value::Ptr result);
		return result;
	}

	RuntimeContext::Ptr context_copy = RuntimeContext::make_ptr<RuntimeContext>(*m_current_context);
	m_current_context = context_to_execute_on;
	EXPR_ACCEPT(expression, *this, Value::Ptr result);
	m_current_context = context_copy;
	return result;
}

void Runtime::visit_access_expr(AccessExpr& access_expr) {
	EXPR_ACCEPT(access_expr.get_owner(), *this, Value::Ptr owner);
	const auto& klass = Value::as<Klass>(owner);
	const auto& var = klass->get_instance_context()->get_var(access_expr.get_name().get_value().as_string());
	if(!var.has_value())
		assert(false);
	EXPR_RETURN_FROM_VISIT(var->value);
}

void Runtime::visit_assignment_expr(AssignmentExpr& assignment_expr) {
	EXPR_ACCEPT(assignment_expr.get_new_value(), *this, Value::Ptr new_value);
	if(!m_current_context->mutate_var(assignment_expr.get_name().get_value().as_string(), new_value))
		assert(false);
	EXPR_RETURN_FROM_VISIT(new_value);
}

void Runtime::visit_binary_expr(BinaryExpr& binary_expr) {
	EXPR_ACCEPT(binary_expr.get_lhs(), *this, Value::Ptr lhs);
	EXPR_ACCEPT(binary_expr.get_rhs(), *this, Value::Ptr rhs);
	if(!lhs->is_native()) {
		const auto& g_function =
				Value::as<Klass>(lhs)->get_declaration("operator" + binary_expr.get_operator().get_lexeme());
		auto function = Value::as<Function>(g_function);
		const auto result = function->exec({rhs});
		EXPR_RETURN_FROM_VISIT(result);
	}
	switch(binary_expr.get_operator().get_type()) {
		case TokenType::EQUAL_EQUAL: EXPR_RETURN_FROM_VISIT(Value::make_ptr<Bool>(*lhs == rhs));
		case TokenType::BANG_EQUAL: EXPR_RETURN_FROM_VISIT(Value::make_ptr<Bool>(*lhs != rhs));
		case TokenType::GREATER: EXPR_RETURN_FROM_VISIT(Value::make_ptr<Bool>(*lhs > rhs));
		case TokenType::GREATER_EQUAL: EXPR_RETURN_FROM_VISIT(Value::make_ptr<Bool>(*lhs >= rhs));
		case TokenType::LESS: EXPR_RETURN_FROM_VISIT(Value::make_ptr<Bool>(*lhs < rhs));
		case TokenType::LESS_EQUAL: EXPR_RETURN_FROM_VISIT(Value::make_ptr<Bool>(*lhs <= rhs));
		case TokenType::MINUS: EXPR_RETURN_FROM_VISIT(*lhs - rhs);
		case TokenType::PLUS: EXPR_RETURN_FROM_VISIT(*lhs + rhs);
		case TokenType::STAR: EXPR_RETURN_FROM_VISIT(*lhs * rhs);
		case TokenType::SLASH: EXPR_RETURN_FROM_VISIT(*lhs / rhs);
		default: assert(false);
	}
}

void Runtime::visit_call_expr(CallExpr& call_expr) {
	EXPR_ACCEPT(call_expr.get_function(), *this, Value::Ptr g_function);
	auto function = Value::as<Function>(g_function);
	assert(call_expr.get_arguments().size() == function->get_arity());

	std::vector<Value::Ptr> args;
	for(const auto& arg : call_expr.get_arguments()) {
		EXPR_ACCEPT(arg, *this, Value::Ptr value);
		args.push_back(value);
	}
	Value::Ptr return_val = function->exec(args);
	EXPR_RETURN_FROM_VISIT(return_val);
}

void Runtime::visit_lambda_function(LambdaFunctionExpr& function_expr) {
	EXPR_RETURN_FROM_VISIT(
			Value::make_ptr<Function>(std::make_shared<LambdaFunctionExpr>(function_expr), m_current_context));
}

void Runtime::visit_group_expr(GroupExpr& group_expr) {
	EXPR_ACCEPT(group_expr.get_expr(), *this, Value::Ptr value);
	EXPR_RETURN_FROM_VISIT(value);
}

void Runtime::visit_instantiation_expr(InstantiationExpr& instantiation_expr) {
	const auto& type_or_error =
			m_current_context->get_type(instantiation_expr.get_class_name().get_value().as_string());
	if(!type_or_error.has_value())
		assert(false);
	Value::Ptr klass = Value::make_ptr<Klass>(*Value::as<Klass>(*type_or_error));
	std::vector<Value::Ptr> args;
	for(const auto& arg : instantiation_expr.get_arguments()) {
		EXPR_ACCEPT(arg, *this, Value::Ptr value);
		args.emplace_back(value);
	}
	Value::as<Klass>(klass)->instantiate(args);
	EXPR_RETURN_FROM_VISIT(klass);
}

void Runtime::visit_literal(LiteralExpr& literal_expr) {
	switch(literal_expr.get_type()) {
		case LiteralExpr::t_int: EXPR_RETURN_FROM_VISIT(Value::make_ptr<Number>(literal_expr.get_int_value())); break;
		case LiteralExpr::t_string:
			EXPR_RETURN_FROM_VISIT(Value::make_ptr<String>(literal_expr.get_string_value()));
			break;
		case LiteralExpr::t_bool: EXPR_RETURN_FROM_VISIT(Value::make_ptr<Bool>(literal_expr.get_bool_value())); break;
		case LiteralExpr::t_nothing: EXPR_RETURN_FROM_VISIT(Value::make_ptr<Nothing>()); break;
	}
}

void Runtime::visit_type_expr(TypeExpr&) {
	// a type expr. has no effect during runtime
}

void Runtime::visit_unary_expr(UnaryExpr& unary_expr) {
	EXPR_ACCEPT(unary_expr.get_rhs(), *this, Value::Ptr value);
	switch(unary_expr.get_operator().get_type()) {
		case TokenType::BANG: EXPR_RETURN_FROM_VISIT(Value::make_ptr<Bool>(!value->is_implicitly_true()));
		case TokenType::MINUS: EXPR_RETURN_FROM_VISIT(*value * Value::make_ptr<Number>(-1));
		default: assert(false);
	}
}

void Runtime::visit_variable(VariableExpr& variable_expr) {
	const std::string name = variable_expr.get_name().get_value().as_string();
	const auto& var = m_current_context->get_var(name);
	const auto& fun = m_current_context->get_function(name);
	if(!var.has_value() && !fun.has_value())
		assert(false);
	EXPR_RETURN_FROM_VISIT(var.has_value() ? var->value : fun->value);
}

void Runtime::visit_block_stmt(BlockStmt& block_stmt) {
	run_in_new_context(
			[&block_stmt, this]() {
				for(const auto& statement : block_stmt.get_statements())
					statement->accept(*this);
			},
			m_current_context);
}

void Runtime::visit_var_declaration_stmt(VarDeclarationStmt& declaration_stmt) {
	Value::Ptr init = Value::make_ptr<Nothing>();
	if(declaration_stmt.get_initializer() != nullptr) {
		EXPR_ACCEPT(declaration_stmt.get_initializer(), *this, init);
	}

	const std::string& name = declaration_stmt.get_identifier().get_value().as_string();
	if(!m_current_context->declare_var(name, init, declaration_stmt.is_mutable()))
		assert(false);
}

void Runtime::visit_class_declaration_stmt(ClassDeclarationStmt& class_declaration_stmt) {
	const std::string& name = class_declaration_stmt.get_class_name().get_value().as_string();
	if(!m_current_context->declare_type(name, Value::make_ptr<Klass>(class_declaration_stmt)))
		assert(false);
}

void Runtime::visit_fun_declaration_stmt(FunDeclarationStmt& fun_declaration_stmt) {
	const std::string& name = fun_declaration_stmt.get_identifier().get_value().as_string();
	EXPR_ACCEPT(fun_declaration_stmt.get_function(), *this, Value::Ptr function);
	if(!m_current_context->declare_function(name, Variable(function, false)))
		assert(false);
}

void Runtime::visit_expression_stmt(ExpressionStmt& expression_stmt) { expression_stmt.get_expr()->accept(*this); }

void Runtime::visit_print_stmt(PrintStmt& print_stmt) {
	EXPR_ACCEPT(print_stmt.get_expr(), *this, Value::Ptr value);
	std::cout << value->to_string() << "\n";
}

void Runtime::visit_return_stmt(ReturnStmt& return_stmt) {
	EXPR_ACCEPT(return_stmt.get_expr(), *this, Value::Ptr value);
	throw ReturnException(value);
}

void Runtime::visit_while_stmt(WhileStmt& while_stmt) {
	EXPR_ACCEPT(while_stmt.get_condition(), *this, Value::Ptr condition);
	while(condition->is_implicitly_true()) {
		while_stmt.get_statement()->accept(*this);
		EXPR_ACCEPT(while_stmt.get_condition(), *this, condition);
	}
}

void Runtime::visit_if_stmt(IfStmt& if_stmt) {
	EXPR_ACCEPT(if_stmt.get_condition(), *this, Value::Ptr condition);
	if(condition->is_implicitly_true())
		if_stmt.get_then()->accept(*this);
	else if(if_stmt.get_else() != nullptr)
		if_stmt.get_else()->accept(*this);
}

template <typename Callback>
RuntimeContext::Ptr Runtime::run_in_new_context(const Callback& callback, RuntimeContext::Ptr parent) {
	RuntimeContext::Ptr context_copy = std::move(m_current_context);
	m_current_context = RuntimeContext::make_ptr<RuntimeContext>(std::move(parent));
	try {
		callback();
	} catch(const ReturnException&) {
		RuntimeContext::Ptr modified_context = std::move(m_current_context);
		m_current_context = std::move(context_copy);
		throw;
	}
	RuntimeContext::Ptr modified_context = std::move(m_current_context);
	m_current_context = std::move(context_copy);
	return modified_context;
}
}