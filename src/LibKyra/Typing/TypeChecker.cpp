#include "TypeChecker.hpp"

#include <string>
#include <utility>

#include "../Expressions/AccessExpr.hpp"
#include "../Expressions/AssignmentExpr.hpp"
#include "../Expressions/BinaryExpr.hpp"
#include "../Expressions/CallExpr.hpp"
#include "../Expressions/FunctionExpr.hpp"
#include "../Expressions/GroupExpr.hpp"
#include "../Expressions/InstantiationExpr.hpp"
#include "../Expressions/LiteralExpr.hpp"
#include "../Expressions/TypeExpr.hpp"
#include "../Expressions/UnaryExpr.hpp"
#include "../Expressions/VariableExpr.hpp"
#include "../Statements/BlockStmt.hpp"
#include "../Statements/ClassDeclarationStmt.hpp"
#include "../Statements/DeclarationStmt.hpp"
#include "../Statements/ExpressionStmt.hpp"
#include "../Statements/IfStmt.hpp"
#include "../Statements/PrintStmt.hpp"
#include "../Statements/ReturnStmt.hpp"
#include "../Statements/WhileStmt.hpp"
#include "ClassType.hpp"
#include "FunctionType.hpp"

namespace Kyra {
TypeChecker& TypeChecker::the() {
	static TypeChecker instance;
	return instance;
}

TypeChecker::Result TypeChecker::check(const std::vector<Statement::Ptr>& statements) {
	TypeChecker::Result result;
	for(const auto& statement : statements)
		check(statement);

	for(const auto& error : m_errors)
		result.insert_error(error);

	m_errors.clear();
	return result;
}

void TypeChecker::check(const Statement::Ptr& statement) { statement->accept(*this); }

template <class Callback>
TypeContext::Ptr TypeChecker::run_in_new_context(const Callback& function,
		TypeContext::Ptr parent,
		const TypeContext::Ptr& values_to_copy) {
	TypeContext::Ptr context_copy = std::move(m_current_context);
	m_current_context = TypeContext::make_ptr<TypeContext>(std::move(parent));
	if(values_to_copy != nullptr) {
		for(auto [name, variable] : values_to_copy->get_variables())
			m_current_context->declare_var(name, std::move(variable));
		for(auto [type_name, type] : values_to_copy->get_types())
			m_current_context->declare_type(type_name, std::move(type));
	}
	function();
	TypeContext::Ptr modified_context = std::move(m_current_context);
	m_current_context = std::move(context_copy);
	return modified_context;
}

void TypeChecker::visit_access_expr(AccessExpr& access_expr) {
	EXPR_ACCEPT(access_expr.get_owner(), *this, Type::Ptr owner_type);
	const std::string& name = access_expr.get_name().get_value().as_string();
	const auto& result = owner_type->knows_about(name);
	if(!result.has_value())
		THROW_TYPING_ERROR(UndefinedMemberError(access_expr.get_position(), owner_type->get_name(), name));
	EXPR_RETURN_FROM_VISIT(result->value);
}

void TypeChecker::visit_assignment_expr(AssignmentExpr& assignment_expr) {
	Type::Ptr assigned_value_type;
	const std::string& name = assignment_expr.get_name().get_value().as_string();

	if(assignment_expr.get_owner() != nullptr) {
		EXPR_ACCEPT(assignment_expr.get_owner(), *this, Type::Ptr owner_type);
		assigned_value_type = owner_type->knows_about(name)->value;
	} else
		assigned_value_type = m_current_context->get_var(name)->value;

	if(assigned_value_type == nullptr)
		THROW_TYPING_ERROR(UndefinedVariableError(assignment_expr.get_position(), name));
	if(!m_current_context->get_var(name)->is_mutable)
		THROW_TYPING_ERROR(AssignmentToConstError(assignment_expr.get_position(), name));
	EXPR_ACCEPT(assignment_expr.get_new_value(), *this, Type::Ptr new_value_type);
	if(!new_value_type->can_be_assigned_to(assigned_value_type))
		THROW_TYPING_ERROR(WrongTypeError(assignment_expr.get_position(),
				assigned_value_type->get_name(),
				new_value_type->get_name()));
	EXPR_RETURN_FROM_VISIT(new_value_type);
}

void TypeChecker::visit_binary_expr(BinaryExpr& binary_expr) {
	EXPR_ACCEPT(binary_expr.get_lhs(), *this, Type::Ptr lhs);
	EXPR_ACCEPT(binary_expr.get_rhs(), *this, Type::Ptr rhs);

	const std::string& expected_function_name = "operator" + binary_expr.get_operator().get_lexeme();
	if(const auto& res = lhs->knows_about(expected_function_name); res.has_value()) {
		Type::Ptr type = res->value;
		if(type->can_be_called_with({rhs}))
			EXPR_RETURN_FROM_VISIT(std::dynamic_pointer_cast<FunctionType>(type)->get_return_type());
	}
	THROW_TYPING_ERROR(UndefinedMemberError(binary_expr.get_position(), lhs->get_name(), expected_function_name));
}

void TypeChecker::visit_call_expr(CallExpr& call_expr) {
	EXPR_ACCEPT(call_expr.get_function(), *this, Type::Ptr g_function);
	const auto& function = std::dynamic_pointer_cast<FunctionType>(g_function);
	if(g_function == nullptr)
		return;
	if(function == nullptr)
		THROW_TYPING_ERROR(
				WrongTypeError(call_expr.get_position(), Value::NativeTypes::Function, g_function->get_name()));
	if(function->get_parameters().size() != call_expr.get_arguments().size())
		THROW_TYPING_ERROR(ArityError(call_expr.get_position(),
				function->get_arity(),
				call_expr.get_arguments().size(),
				function->get_name()));
	for(unsigned long i = 0; i < function->get_parameters().size(); ++i) {
		Type::Ptr param = function->get_parameters()[i];
		EXPR_ACCEPT(call_expr.get_arguments()[i], *this, Type::Ptr arg);
		if(!arg->can_be_assigned_to(param))
			THROW_TYPING_ERROR(WrongTypeError(call_expr.get_position(), param->get_name(), arg->get_name()));
	}

	EXPR_RETURN_FROM_VISIT(function->get_return_type());
}

void TypeChecker::visit_function(FunctionExpr& function_expr) {
	FunctionType* enclosing_function_type = m_current_function;
	EXPR_ACCEPT(function_expr.get_return_type(), *this, Type::Ptr return_type);
	std::vector<Type::Ptr> parameters;
	TypeContext::Ptr param_scope = run_in_new_context(
			[&function_expr, &parameters, this]() {
				for(const auto& param : function_expr.get_parameters()) {
					EXPR_ACCEPT(param.type, *this, Type::Ptr param_type);
					if(!param_type->is_applicable_for_declaration())
						THROW_TYPING_ERROR(UndefinedTypeError(function_expr.get_position(), param_type->get_name()));
					parameters.push_back(param_type);
					if(!m_current_context->declare_var(param.name.get_value().as_string(), param_type, true))
						THROW_TYPING_ERROR(AlreadyDefinedVariableError(function_expr.get_position(),
								param.name.get_value().as_string()));
				}
			},
			m_current_context);

	std::shared_ptr<FunctionType> function_type = std::make_shared<FunctionType>(return_type, std::move(parameters));

	m_current_function = function_type.get();
	run_in_new_context([&function_expr, this]() { function_expr.get_implementation()->accept(*this); },
			m_current_context,
			param_scope);
	m_current_function = enclosing_function_type;

	if(!m_does_current_function_return && function_type->get_return_type()->get_name() != Value::NativeTypes::Nothing)
		THROW_TYPING_ERROR(WrongTypeError(function_expr.get_position(),
				function_type->get_return_type()->get_name(),
				Value::NativeTypes::Nothing));

	m_does_current_function_return = false;
	EXPR_RETURN_FROM_VISIT(function_type);
}

void TypeChecker::visit_group_expr(GroupExpr& group_expr) {
	EXPR_ACCEPT(group_expr.get_expr(), *this, Type::Ptr type);
	EXPR_RETURN_FROM_VISIT(type);
}

void TypeChecker::visit_instantiation_expr(InstantiationExpr& instantiation_expr) {
	const auto& result = m_current_context->get_type(instantiation_expr.get_class_name().get_value().as_string());
	if(!result.has_value())
		THROW_TYPING_ERROR(UndefinedTypeError(instantiation_expr.get_position(),
				instantiation_expr.get_class_name().get_value().as_string()));
	auto class_type = std::dynamic_pointer_cast<ClassType>(*result);
	if(instantiation_expr.get_arguments().size() != class_type->get_arity())
		THROW_TYPING_ERROR(ArityError(instantiation_expr.get_position(),
				class_type->get_arity(),
				instantiation_expr.get_arguments().size(),
				"Constructor of class " + instantiation_expr.get_class_name().get_value().as_string()));
	for(unsigned long i = 0; i < class_type->get_arity(); ++i) {
		Type::Ptr expected = class_type->get_constructor_parameter()[i];
		EXPR_ACCEPT(instantiation_expr.get_arguments()[i], *this, Type::Ptr provided);
		if(*expected != provided)
			THROW_TYPING_ERROR(
					WrongTypeError(instantiation_expr.get_position(), expected->get_name(), provided->get_name()));
	}
	EXPR_RETURN_FROM_VISIT(*result);
}

void TypeChecker::visit_literal(LiteralExpr& literal_expr) {
	EXPR_RETURN_FROM_VISIT(*m_current_context->get_type(literal_expr.get_value()->get_type()));
}

void TypeChecker::visit_type_expr(TypeExpr& type_expr) {
	if(type_expr.is_function()) {
		EXPR_ACCEPT(type_expr.get_return_type(), *this, Type::Ptr return_type);
		std::vector<Type::Ptr> param_types;
		for(const auto& param : type_expr.get_parameter_types()) {
			EXPR_ACCEPT(param, *this, Type::Ptr param_type);
			param_types.push_back(std::move(param_type));
		}
		EXPR_RETURN_FROM_VISIT(Type::make_ptr<FunctionType>(return_type, std::move(param_types)));
	}
	const auto& result = m_current_context->get_type(type_expr.get_name());
	if(!result.has_value())
		THROW_TYPING_ERROR(UndefinedTypeError(type_expr.get_position(), type_expr.get_name()));
	EXPR_RETURN_FROM_VISIT(*result);
}

// FIXME: this does not work, as there is no operator overloading
//		a type can't have the infix operator AND prefix operator +
void TypeChecker::visit_unary_expr(UnaryExpr& unary_expr) {
	EXPR_ACCEPT(unary_expr.get_rhs(), *this, Type::Ptr rhs);

	const std::string expected_function_name = "operator" + unary_expr.get_operator().get_lexeme();
	if(const auto& res = rhs->knows_about(expected_function_name); res.has_value()) {
		Type::Ptr type = res->value;
		if(type->can_be_called_with({}))
			EXPR_RETURN_FROM_VISIT(std::dynamic_pointer_cast<FunctionType>(type)->get_return_type());
	}
	THROW_TYPING_ERROR(UndefinedMemberError(unary_expr.get_position(), rhs->get_name(), expected_function_name));
}

void TypeChecker::visit_variable(VariableExpr& variable_expr) {
	const std::string& name = variable_expr.get_name().get_value().as_string();
	const auto& result = m_current_context->get_var(name);
	if(!result.has_value()) {
		if(m_current_class_name != nullptr)
			THROW_TYPING_ERROR(
					UndefinedMemberError(variable_expr.get_position(), std::string(m_current_class_name), name));
		else
			THROW_TYPING_ERROR(UndefinedVariableError(variable_expr.get_position(), name));
	}
	EXPR_RETURN_FROM_VISIT(result->value);
}

void TypeChecker::visit_block_stmt(BlockStmt& block_stmt) {
	run_in_new_context(
			[this, &block_stmt]() {
				for(const auto& statement : block_stmt.get_statements())
					statement->accept(*this);
			},
			m_current_context);
}

void TypeChecker::visit_declaration_stmt(DeclarationStmt& declaration_stmt) {
	const std::string& name = declaration_stmt.get_identifier().get_value().as_string();

	Type::Ptr expected_type{};
	if(declaration_stmt.get_type() == nullptr) {
		if(declaration_stmt.get_initializer() == nullptr)
			THROW_TYPING_ERROR(TypingError(declaration_stmt.get_position(),
					"Variable " + name + " needs to be explicitly typed, as it does not get initialized immediately"));
		EXPR_ACCEPT(declaration_stmt.get_initializer(), *this, expected_type);
	} else {
		EXPR_ACCEPT(declaration_stmt.get_type(), *this, expected_type);
		if(!expected_type->is_applicable_for_declaration())
			THROW_TYPING_ERROR(UndefinedTypeError(declaration_stmt.get_position(), expected_type->get_name()));
		if(declaration_stmt.get_initializer() != nullptr) {
			if(expected_type->is_function())
				m_current_context->declare_var(name, expected_type, declaration_stmt.is_mutable());
			EXPR_ACCEPT(declaration_stmt.get_initializer(), *this, Type::Ptr init_type);
			if(expected_type->is_function())
				m_current_context->remove_var(name);
			if(!init_type->can_be_assigned_to(expected_type))
				THROW_TYPING_ERROR(WrongTypeError(declaration_stmt.get_position(),
						expected_type->get_name(),
						init_type->get_name()));
		}
	}
	if(!m_current_context->declare_var(name, expected_type, declaration_stmt.is_mutable())) {
		if(m_current_class_name != nullptr)
			THROW_TYPING_ERROR(AlreadyDefinedMemberError(declaration_stmt.get_position(),
					std::string(m_current_class_name),
					name));
		else
			THROW_TYPING_ERROR(AlreadyDefinedVariableError(declaration_stmt.get_position(), name));
	}
}

void TypeChecker::visit_class_declaration_stmt(ClassDeclarationStmt& class_declaration_stmt) {
	m_current_class_name = class_declaration_stmt.get_class_name().get_value().as_string().data();
	const std::string& name = class_declaration_stmt.get_class_name().get_value().as_string();
	if(m_current_context->get_type(name).has_value())
		THROW_TYPING_ERROR(AlreadyDefinedTypeError(class_declaration_stmt.get_position(), name));

	auto* klass = new ClassType(name);
	m_current_context->declare_type(name, ClassType::Ptr(klass));

	TypeContext::Ptr param_scope = run_in_new_context(
			[&class_declaration_stmt, &klass, this]() {
				for(const auto& param : class_declaration_stmt.get_constructor_parameters()) {
					EXPR_ACCEPT(param.type, *this, Type::Ptr param_type);
					if(!param_type->is_applicable_for_declaration())
						THROW_TYPING_ERROR(
								UndefinedTypeError(class_declaration_stmt.get_position(), param_type->get_name()));
					klass->add_constructor_param(param_type);
					klass->add_declaration(param.name.get_value().as_string(),
							Variable<Type::Ptr>(param_type, param.is_mutable));
					if(!m_current_context->declare_var(param.name.get_value().as_string(),
							   param_type,
							   param.is_mutable))
						THROW_TYPING_ERROR(AlreadyDefinedVariableError(class_declaration_stmt.get_position(),
								param.name.get_value().as_string()));
				}
			},
			m_current_context);

	TypeContext::Ptr decl_scope = run_in_new_context(
			[&class_declaration_stmt, this]() {
				for(const auto& decl : class_declaration_stmt.get_declarations())
					decl->accept(*this);
			},
			m_current_context,
			param_scope);

	for(const auto& [decl_name, decl] : decl_scope->get_variables())
		klass->add_declaration(decl_name, decl);

	m_current_class_name = nullptr;
}
void TypeChecker::visit_expression_stmt(ExpressionStmt& expression_stmt) { expression_stmt.get_expr()->accept(*this); }

void TypeChecker::visit_print_stmt(PrintStmt& print_stmt) { EXPR_ACCEPT(print_stmt.get_expr(), *this, Type::Ptr type); }

void TypeChecker::visit_return_stmt(ReturnStmt& return_stmt) {
	if(m_current_function == nullptr)
		THROW_TYPING_ERROR(InvalidReturnError(return_stmt.get_position()));
	EXPR_ACCEPT(return_stmt.get_expr(), *this, Type::Ptr returned_type);
	if(*returned_type != m_current_function->get_return_type())
		THROW_TYPING_ERROR(WrongTypeError(return_stmt.get_position(),
				m_current_function->get_return_type()->get_name(),
				returned_type->get_name()));
	m_does_current_function_return = true;
}
void TypeChecker::visit_while_stmt(WhileStmt& while_stmt) {
	EXPR_ACCEPT(while_stmt.get_condition(), *this, Type::Ptr condition);
	while_stmt.get_statement()->accept(*this);
}

void TypeChecker::visit_if_stmt(IfStmt& if_stmt) {
	EXPR_ACCEPT(if_stmt.get_condition(), *this, Type::Ptr condition);
	if_stmt.get_then()->accept(*this);
	if(if_stmt.get_else() != nullptr)
		if_stmt.get_else()->accept(*this);
}
}