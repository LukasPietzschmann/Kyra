#include "TypeChecker.hpp"

#include <iostream>
#include <sstream>

namespace Kyra {
using namespace Untyped;

ErrorOr<std::vector<RefPtr<Typed::Statement>>> TypeChecker::check_statements(
	const std::vector<RefPtr<Statement>>& statements) {
	m_typed_statements.clear();
	try {
		for(const RefPtr<Statement>& statement : statements)
			statement->accept(*this);
	} catch(const ErrorException& e) {
		return e;
	}
	return m_typed_statements;
}

void TypeChecker::visit(const ExpressionStatement& expresion_statement) {
	RefPtr<Typed::Expression> expr = visit_with_return(expresion_statement.get_expression()).expression;
	m_typed_statements.push_back(mk_ref<Typed::ExpressionStatement>(expr));
}

void TypeChecker::visit(const Declaration& declaration) {
	const std::string_view name = declaration.get_identifier().get_lexeme();
	RefPtr<DeclaredType> declared_type = visit_with_return(declaration.get_type()).type->get_declared_type_shared();
	bool is_mutable = declaration.get_declaration_kind() == Declaration::Kind::VAR;
	RefPtr<AppliedType> applied_type = AppliedType::promote_declared_type(declared_type, is_mutable);
	RefPtr<Typed::Expression> typed_init = nullptr;
	if(const Expression* init = declaration.get_initializer(); init != nullptr) {
		auto [init_type, expr] = visit_with_return(*init);
		typed_init = expr;
		if(!init_type->can_be_assigned_to(*applied_type))
			throw ErrorException("Initializer type does not match declaration type", init->get_source_range());
	} else if(!is_mutable)
		throw ErrorException("Values have to be initialized during declaration", declaration.get_source_range());

	DeclarationDumpster::the().abort_on_exception([&]() {
		declid_t decl_id = DeclarationDumpster::the().insert({name, applied_type});
		bool successful = m_current_scope->insert_symbol(name, {decl_id, applied_type});
		if(!successful)
			throw ErrorException("Symbol already declared", declaration.get_identifier().get_source_range());
		m_typed_statements.push_back(mk_ref<Typed::Declaration>(decl_id, typed_init));
	});
}

void TypeChecker::visit(const Structure& structure) {
	const Token& identifier = structure.get_identifier();
	if(m_current_scope->find_type(identifier.get_lexeme()) != nullptr)
		throw ErrorException("Redefinition of struct", identifier.get_source_range());
	std::vector<RefPtr<Typed::Declaration>> declarations;
	std::vector<std::pair<std::string_view, Element<AppliedType>>> declaration_types;
	execute_on_new_scope([&]() {
		for(const RefPtr<Declaration>& decl : structure.get_declarations()) {
			const std::vector<RefPtr<Typed::Declaration>>& statements =
				capture_typed_statements<Typed::Declaration>([&]() { decl->accept(*this); });
			declarations.push_back(statements.back());
			auto [name, type] = DeclarationDumpster::the().retrieve(statements.back()->get_declaration_id());
			declaration_types.emplace_back(name, Element<AppliedType>{statements.back()->get_declaration_id(), type});
		}
	});
	RefPtr<DeclaredType> new_type = mk_ref<StructType>(identifier.get_lexeme(), declaration_types);
	m_current_scope->insert_type(identifier.get_lexeme(), new_type);
	m_typed_statements.push_back(mk_ref<Typed::Structure>(identifier.get_lexeme(), declarations));
}

void TypeChecker::visit(const Function& function) {
	RefPtr<TypeScope> function_scope = mk_ref<TypeScope>(m_current_scope);
	std::vector<RefPtr<AppliedType>> parameters;
	std::vector<declid_t> typed_parameters;
	for(const Function::Parameter& parameter : function.get_parameters()) {
		RefPtr<DeclaredType> param_type = visit_with_return(*parameter.type).type->get_declared_type_shared();
		bool is_mutable = parameter.kind == Declaration::Kind::VAR;
		RefPtr<AppliedType> applied_param_type = AppliedType::promote_declared_type(param_type, is_mutable);
		DeclarationDumpster::the().abort_on_exception([&]() {
			declid_t param_decl_id =
				DeclarationDumpster::the().insert({parameter.identifier.get_lexeme(), applied_param_type});
			typed_parameters.push_back(param_decl_id);
			bool successful =
				function_scope->insert_symbol(parameter.identifier.get_lexeme(), {param_decl_id, applied_param_type});
			if(!successful)
				throw ErrorException("Parameter already declared", parameter.identifier.get_source_range());
			parameters.push_back(applied_param_type);
		});
	}
	RefPtr<DeclaredType> return_type = visit_with_return(function.get_return_type()).type->get_declared_type_shared();
	RefPtr<FunctionType> function_type =
		mk_ref<FunctionType>(function.get_identifier().get_lexeme(), return_type, parameters);
	m_context.enclosing_function = function_type;
	const std::vector<RefPtr<Typed::Block>>& statements = capture_typed_statements<Typed::Block>(
		[&]() { execute_on_scope(function_scope, [&]() { function.get_implementation().accept(*this); }); });
	assert(statements.size() == 1);
	m_context.enclosing_function = nullptr;
	if(!m_context.had_return)
		throw ErrorException("Missing return statement", function.get_implementation().get_source_range());
	DeclarationDumpster::the().abort_on_exception([&]() {
		declid_t fun_decl_id = DeclarationDumpster::the().insert(
			{function.get_identifier().get_lexeme(), AppliedType::promote_declared_type(function_type, false)});
		if(!m_current_scope->insert_function(function.get_identifier().get_lexeme(), {fun_decl_id, function_type}))
			throw ErrorException("Redefinition of function", function.get_identifier().get_source_range());
		m_typed_statements.push_back(mk_ref<Typed::Function>(fun_decl_id, statements.front(), typed_parameters));
	});
	m_context.had_return = false;
}

void TypeChecker::visit(const Print& print_statement) {
	RefPtr<Typed::Expression> return_expr = visit_with_return(print_statement.get_expression()).expression;
	m_typed_statements.push_back(mk_ref<Typed::Print>(return_expr));
}

void TypeChecker::visit(const Return& return_statement) {
	RefPtr<FunctionType> function = m_context.enclosing_function;
	if(function == nullptr)
		throw ErrorException("Return can only be used inside a function", return_statement.get_source_range());

	auto [actual_return_type, return_expr] = visit_with_return(return_statement.get_expression());
	if(!actual_return_type->can_be_assigned_to(
		   *AppliedType::promote_declared_type(function->get_returned_type(), true))) {
		throw ErrorException("Wrong return type", return_statement.get_expression().get_source_range());
	}
	m_typed_statements.push_back(mk_ref<Typed::Return>(return_expr));
	m_context.had_return = true;
}

void TypeChecker::visit(const Block& block) {
	const std::vector<RefPtr<Typed::Statement>>& statements = capture_typed_statements<Typed::Statement>([&]() {
		execute_on_new_scope([&]() {
			for(const RefPtr<Statement>& statement : block.get_body())
				statement->accept(*this);
		});
	});
	m_typed_statements.push_back(mk_ref<Typed::Block>(statements));
}

void TypeChecker::visit(const IntLiteral& literal) {
	RefPtr<DeclaredType> i32_type = m_current_scope->find_type("i32");
	RefPtr<AppliedType> type = AppliedType::promote_declared_type(i32_type, true);
	return_from_visit_emplace(type, mk_ref<Typed::IntLiteral>(type, literal.get_literal_value()));
}

void TypeChecker::visit(const Assignment& assignment) {
	auto [lhs_type, lhs_expr] = visit_with_return(assignment.get_lhs());
	RefPtr<Typed::VarQuery> lhs_var_query = std::static_pointer_cast<Typed::VarQuery>(lhs_expr);

	if(!lhs_type->is_mutable())
		throw ErrorException("Cannot assign to a value", assignment.get_source_range());
	auto [rhs_type, rhs_expr] = visit_with_return(assignment.get_rhs());
	if(!rhs_type->can_be_assigned_to(*lhs_type))
		throw ErrorException("Wrong type", assignment.get_rhs().get_source_range());
	return_from_visit_emplace(
		lhs_type, mk_ref<Typed::Assignment>(lhs_type, lhs_var_query->get_declaration_id(), rhs_expr));
}

void TypeChecker::visit(const BinaryExpression& binary_expression) {
	std::stringstream function_name;
	const Token& oper = binary_expression.get_operator();
	function_name << "operator" << oper.get_lexeme();
	auto [lhs_type, lhs_expr] = visit_with_return(binary_expression.get_lhs());
	auto [rhs_type, rhs_expr] = visit_with_return(binary_expression.get_rhs());
	// TODO: remove ability for methods
	std::vector<Element<FunctionType>> methods = lhs_type->get_declared_type().find_methods(function_name.str());
	if(methods.empty())
		throw ErrorException("Undefined operator", binary_expression.get_operator().get_source_range());

	bool native = true;
	RefPtr<FunctionType> candidate = nullptr;
	for(const auto& [_, method] : methods) {
		if(!method->can_be_called_with({rhs_type}))
			continue;
		candidate = method;
	}
	if(candidate == nullptr) {
		methods = m_current_scope->find_functions(function_name.str());
		for(const auto& [_, method] : methods) {
			if(!method->can_be_called_with({lhs_type, rhs_type}))
				continue;
			candidate = method;
			native = false;
		}
	}
	if(candidate == nullptr)
		throw ErrorException("No candidate matched", binary_expression.get_operator().get_source_range());
	RefPtr<AppliedType> type = AppliedType::promote_declared_type(candidate->get_returned_type(), true);
	if(!native) {
		// TODO: get declid
		return_from_visit_emplace(
			type, mk_ref<Typed::Call>(type, 12, std::vector<RefPtr<Typed::Expression>>{lhs_expr, rhs_expr}));
	}
	return_from_visit_emplace(type, mk_ref<Typed::BinaryExpression>(type, lhs_expr, rhs_expr, oper));
}

void TypeChecker::visit(const TypeIndicator& type) {
	RefPtr<DeclaredType> found_type = m_current_scope->find_type(type.get_type().get_lexeme());
	if(found_type == nullptr)
		throw ErrorException("Undefined type", type.get_source_range());
	// TypeIndicatior does not exist in the Typed namespace, so no Typed::Expression is created here
	return_from_visit_emplace(AppliedType::promote_declared_type(found_type, true), nullptr);
}

void TypeChecker::visit(const Call& call) {
	const auto& functions = m_current_scope->find_functions(call.get_function_name().get_lexeme());
	if(functions.empty())
		throw ErrorException("Undefined function", call.get_function_name().get_source_range());
	std::vector<RefPtr<AppliedType>> arg_types;
	std::vector<RefPtr<Typed::Expression>> arg_exprs;
	for(const Call::Argument& arg : call.get_arguments()) {
		auto [type, expr] = visit_with_return(*arg.value);
		arg_types.push_back(type);
		arg_exprs.push_back(expr);
	}
	Element<FunctionType> candidate = {0, nullptr};
	for(const auto& function : functions) {
		if(!function.type->can_be_called_with(arg_types))
			continue;
		candidate = function;
	}
	if(candidate.type == nullptr)
		throw ErrorException("No candidate matched", call.get_function_name().get_source_range());
	RefPtr<AppliedType> type = AppliedType::promote_declared_type(candidate.type->get_returned_type(), true);
	// FIXME: return functions return type
	return_from_visit_emplace(type, mk_ref<Typed::Call>(type, candidate.declid, arg_exprs));
}

void TypeChecker::visit(const Group& group) { return_from_visit(visit_with_return(group.get_content())); }

void TypeChecker::visit(const VarQuery& var_query) {
	const std::string_view name = var_query.get_identifier().get_lexeme();
	if(const Expression* owner = var_query.get_owner(); owner != nullptr) {
		auto [type, expr] = visit_with_return(*owner);
		std::optional<Element<AppliedType>> var = type->get_declared_type().find_symbol(name);
		if(!var.has_value())
			throw ErrorException("Undefined member", var_query.get_identifier().get_source_range());
		return_from_visit_emplace(var->type, mk_ref<Typed::VarQuery>(var->type, var->declid));
	}
	auto element_or_none = m_current_scope->find_symbol(name);
	if(!element_or_none.has_value())
		throw ErrorException("Undefined symbol", var_query.get_source_range());
	auto [decl_id, type] = element_or_none.value();
	return_from_visit_emplace(type, mk_ref<Typed::VarQuery>(type, decl_id));
}

template <typename Callback>
void TypeChecker::execute_on_scope(RefPtr<TypeScope> scope, Callback callback) {
	RefPtr<TypeScope> backup = m_current_scope;
	m_current_scope = std::move(scope);
	callback();
	m_current_scope = backup;
}

template <typename Callback>
RefPtr<TypeScope> TypeChecker::execute_on_new_scope(Callback callback) {
	RefPtr<TypeScope> new_scope = mk_ref<TypeScope>(m_current_scope);
	execute_on_scope(new_scope, callback);
	return new_scope;
}

template <typename RT, typename Callback>
std::vector<RefPtr<RT>> TypeChecker::capture_typed_statements(Callback callback) {
	unsigned start_index = m_typed_statements.size();
	callback();
	std::vector<RefPtr<RT>> result;
	for(auto it = m_typed_statements.begin() + start_index; it != m_typed_statements.end(); ++it)
		result.push_back(std::static_pointer_cast<RT>(*it));
	m_typed_statements.erase(m_typed_statements.begin() + start_index, m_typed_statements.end());
	return result;
}
}
