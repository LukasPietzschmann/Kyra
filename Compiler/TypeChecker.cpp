#include "TypeChecker.hpp"

#include <sstream>

DeclaredType::DeclaredType(std::string_view name) : m_name(name) {}

bool DeclaredType::can_be_assigned_to(const DeclaredType& other) const { return m_name == other.m_name; }

const std::string_view DeclaredType::get_name() const { return m_name; }

AppliedType::AppliedType(RefPtr<DeclaredType> declared_type, bool is_mutable) :
	m_decl_type(declared_type), m_is_multable(is_mutable) {}

bool AppliedType::can_be_assigned_to(const AppliedType& other) const {
	if(!m_decl_type->can_be_assigned_to(*other.m_decl_type))
		return false;
	// var can be assigned to val and var
	// val can only be assigned to val
	return !(!m_is_multable && other.m_is_multable);
}

const std::vector<RefPtr<FunctionType>> DeclaredType::find_methods(std::string_view name) const {
	if(const auto& it = m_methods.find(name); it != m_methods.end())
		return it->second;
	return {};
}

void DeclaredType::insert_method_if_non_exists(std::string_view name, RefPtr<FunctionType> type) {
	std::vector<RefPtr<FunctionType>>& methods = m_methods[name];
	for(const RefPtr<FunctionType>& method : methods) {
		if(*method == *type)
			return;
	}
	methods.push_back(type);
}

const DeclaredType& AppliedType::get_declared_type() const { return *m_decl_type; }

RefPtr<DeclaredType> AppliedType::get_declared_type_shared() const { return m_decl_type; }

bool AppliedType::is_mutable() const { return m_is_multable; }

FunctionType::FunctionType(
	std::string_view name, RefPtr<DeclaredType> return_type, const std::vector<RefPtr<AppliedType>>& parameters) :
	DeclaredType(name),
	m_return_type(return_type), m_parameters(parameters) {}

RefPtr<DeclaredType> FunctionType::get_returned_type() const { return m_return_type; }

const std::vector<RefPtr<AppliedType>>& FunctionType::get_parameter() const { return m_parameters; }

bool FunctionType::can_be_called_with(const std::vector<RefPtr<AppliedType>>& arguments) const {
	if(arguments.size() != m_parameters.size())
		return false;
	for(unsigned i = 0; i < arguments.size(); ++i) {
		if(arguments.at(i)->can_be_assigned_to(*m_parameters.at(i)))
			continue;
		return false;
	}
	return true;
}

bool FunctionType::operator==(const FunctionType& other) const {
	if(!other.m_return_type->can_be_assigned_to(*m_return_type))
		return false;
	if(m_parameters.size() != other.m_parameters.size())
		return false;
	for(unsigned i = 0; i < m_parameters.size(); ++i) {
		if(other.m_parameters.at(i)->can_be_assigned_to(*m_parameters.at(i)))
			continue;
		return false;
	}
	return true;
}

IntType::IntType(std::string_view name, unsigned width) : DeclaredType(name), m_width(width) {}

unsigned IntType::get_width() const { return m_width; }

TypeScope::TypeScope(RefPtr<TypeScope> parent) : m_parent(parent) {
	static RefPtr<IntType> i32_type = nullptr;
	if(i32_type == nullptr) {
		i32_type = mk_ref<IntType>("i32", 32);
		const std::vector<RefPtr<AppliedType>> rhs = {AppliedType::promote_declared_type(i32_type, false)};
		RefPtr<FunctionType> oper_plus = mk_ref<FunctionType>("operator+", i32_type, rhs);
		RefPtr<FunctionType> oper_minus = mk_ref<FunctionType>("operator-", i32_type, rhs);
		RefPtr<FunctionType> oper_mul = mk_ref<FunctionType>("operator*", i32_type, rhs);
		RefPtr<FunctionType> oper_div = mk_ref<FunctionType>("operator/", i32_type, rhs);
		i32_type->insert_method_if_non_exists(oper_plus->get_name(), oper_plus);
		i32_type->insert_method_if_non_exists(oper_minus->get_name(), oper_minus);
		i32_type->insert_method_if_non_exists(oper_mul->get_name(), oper_mul);
		i32_type->insert_method_if_non_exists(oper_div->get_name(), oper_div);
	}
	m_type_scope.try_emplace(i32_type->get_name(), i32_type);
}

RefPtr<AppliedType> TypeScope::find_symbol(std::string_view name) const {
	if(const auto& it = m_symbol_scope.find(name); it != m_symbol_scope.end())
		return it->second;
	if(m_parent != nullptr)
		return m_parent->find_symbol(name);
	return nullptr;
}

bool TypeScope::insert_symbol(std::string_view name, RefPtr<AppliedType> type) {
	if(m_symbol_scope.contains(name))
		return false;
	m_symbol_scope.try_emplace(name, type);
	return true;
}

RefPtr<DeclaredType> TypeScope::find_type(std::string_view name) const {
	if(const auto& it = m_type_scope.find(name); it != m_type_scope.end())
		return it->second;
	if(m_parent != nullptr)
		return m_parent->find_type(name);
	return nullptr;
}

bool TypeScope::insert_type(std::string_view name, RefPtr<DeclaredType> type) {
	if(m_type_scope.contains(name))
		return false;
	m_type_scope.try_emplace(name, type);
	return true;
}

const std::vector<RefPtr<FunctionType>> TypeScope::find_functions(std::string_view name) const {
	// TODO: find functions from all visible scopes
	if(const auto& it = m_function_scope.find(name); it != m_function_scope.end())
		return it->second;
	if(m_parent != nullptr)
		return m_parent->find_functions(name);
	return {};
}

bool TypeScope::insert_function(std::string_view name, RefPtr<FunctionType> type) {
	std::vector<RefPtr<FunctionType>>& functions = m_function_scope[name];
	for(const RefPtr<FunctionType>& function : functions) {
		if(*function == *type)
			return false;
	}
	functions.push_back(type);
	return true;
}

std::optional<Error> TypeChecker::check_statement(const Statement& statement) {
	try {
		statement.accept(*this);
	} catch(const ErrorException& e) {
		return Error(e);
	}
	return {};
}

void TypeChecker::visit(const ExpressionStatement& expresion_statement) {
	visit_with_return(expresion_statement.get_expression());
}

void TypeChecker::visit(const Declaration& declaration) {
	const std::string_view name = declaration.get_identifier().get_lexeme();
	RefPtr<DeclaredType> declared_type = visit_with_return(declaration.get_type())->get_declared_type_shared();
	bool is_mutable = declaration.get_declaration_kind() == Declaration::Kind::VAR;
	RefPtr<AppliedType> applied_type = AppliedType::promote_declared_type(declared_type, is_mutable);
	if(const Expression* init = declaration.get_initializer(); init != nullptr) {
		RefPtr<AppliedType> init_type = visit_with_return(*init);
		if(!init_type->can_be_assigned_to(*applied_type))
			throw ErrorException("Initializer type does not match declaration type", init->get_source_range());
	}
	bool successful = m_current_scope->insert_symbol(name, applied_type);
	if(!successful)
		throw ErrorException("Symbol already declared", declaration.get_identifier().get_source_range());
}

void TypeChecker::visit(const Function& function) {
	RefPtr<TypeScope> function_scope = mk_ref<TypeScope>(m_current_scope);
	std::vector<RefPtr<AppliedType>> parameters;
	for(const Function::Parameter& parameter : function.get_parameters()) {
		RefPtr<DeclaredType> param_type = visit_with_return(*parameter.type)->get_declared_type_shared();
		bool is_mutable = parameter.kind == Declaration::Kind::VAR;
		RefPtr<AppliedType> applied_param_type = AppliedType::promote_declared_type(param_type, is_mutable);
		function_scope->insert_symbol(parameter.identifier.get_lexeme(), applied_param_type);
		parameters.push_back(applied_param_type);
	}
	RefPtr<DeclaredType> return_type = visit_with_return(function.get_return_type())->get_declared_type_shared();
	RefPtr<FunctionType> function_type =
		mk_ref<FunctionType>(function.get_identifier().get_lexeme(), return_type, parameters);
	m_context.enclosing_function = function_type;
	execute_on_scope(function_scope, [&]() { function.get_implementation().accept(*this); });
	m_context.enclosing_function = nullptr;
	if(!m_current_scope->insert_function(function.get_identifier().get_lexeme(), function_type))
		throw ErrorException("Redefinition of function", function.get_identifier().get_source_range());
}

void TypeChecker::visit(const Return& return_statement) {
	if(RefPtr<FunctionType> function = m_context.enclosing_function; function == nullptr)
		throw ErrorException("Return can only be used inside a function", return_statement.get_source_range());
	else {
		RefPtr<AppliedType> actual_return_type = visit_with_return(return_statement.get_expression());
		if(!actual_return_type->can_be_assigned_to(
			   *AppliedType::promote_declared_type(function->get_returned_type(), true))) {
			throw ErrorException("Wrong return type", return_statement.get_expression().get_source_range());
		}
	}
}

void TypeChecker::visit(const Block& block) {
	execute_on_new_scope([&]() {
		for(const RefPtr<Statement>& statement : block.get_body())
			statement->accept(*this);
	});
}

void TypeChecker::visit(const IntLiteral&) {
	RefPtr<DeclaredType> i32_type = m_current_scope->find_type("i32");
	return_from_visit(AppliedType::promote_declared_type(i32_type, true));
}

void TypeChecker::visit(const Assignment& assignment) {
	RefPtr<AppliedType> assignee_type = m_current_scope->find_symbol(assignment.get_lhs().get_lexeme());
	if(assignee_type == nullptr)
		throw ErrorException("Undefined symbol", assignment.get_lhs().get_source_range());
	RefPtr<AppliedType> rhs_type = visit_with_return(assignment.get_rhs());
	if(!rhs_type->can_be_assigned_to(*assignee_type))
		throw ErrorException("Wrong type", assignment.get_rhs().get_source_range());
	return_from_visit(assignee_type);
}

void TypeChecker::visit(const BinaryExpression& binary_expression) {
	std::stringstream function_name;
	function_name << "operator" << binary_expression.get_operator().get_lexeme();
	RefPtr<AppliedType> lhs_type = visit_with_return(binary_expression.get_lhs());
	RefPtr<AppliedType> rhs_type = visit_with_return(binary_expression.get_rhs());
	const std::vector<RefPtr<FunctionType>> methods = lhs_type->get_declared_type().find_methods(function_name.str());
	if(methods.empty())
		throw ErrorException("Undefined operator", binary_expression.get_operator().get_source_range());

	RefPtr<FunctionType> candidate = nullptr;
	for(const RefPtr<FunctionType>& method : methods) {
		if(!method->can_be_called_with({rhs_type}))
			continue;
		candidate = method;
	}
	if(candidate == nullptr)
		throw ErrorException("No candidate matched", binary_expression.get_operator().get_source_range());
	return_from_visit(AppliedType::promote_declared_type(candidate->get_returned_type(), true));
}

void TypeChecker::visit(const TypeIndicator& type) {
	RefPtr<DeclaredType> found_type = m_current_scope->find_type(type.get_type().get_lexeme());
	if(found_type == nullptr)
		throw ErrorException("Undefined type", type.get_source_range());
	return_from_visit(AppliedType::promote_declared_type(found_type, true));
}

void TypeChecker::visit(const Call& call) {
	const std::vector<RefPtr<FunctionType>>& functions =
		m_current_scope->find_functions(call.get_function_name().get_lexeme());
	if(functions.empty())
		throw ErrorException("Undefined function", call.get_function_name().get_source_range());
	std::vector<RefPtr<AppliedType>> args;
	for(const Call::Argument& arg : call.get_arguments())
		args.push_back(visit_with_return(*arg.value));
	RefPtr<FunctionType> candidate = nullptr;
	for(const RefPtr<FunctionType>& function : functions) {
		if(!function->can_be_called_with(args))
			continue;
		candidate = function;
	}
	if(candidate == nullptr)
		throw ErrorException("No candidate matched", call.get_function_name().get_source_range());
	return_from_visit(AppliedType::promote_declared_type(candidate->get_returned_type(), true));
}

void TypeChecker::visit(const Group& group) { return_from_visit(visit_with_return(group.get_content())); }

void TypeChecker::visit(const VarQuery& var_query) {
	const std::string_view name = var_query.get_identifier().get_lexeme();
	RefPtr<AppliedType> found_type = m_current_scope->find_symbol(name);
	if(found_type == nullptr)
		throw ErrorException("Undefined symbol", var_query.get_source_range());
	return_from_visit(found_type);
}

template <typename Callback>
void TypeChecker::execute_on_scope(RefPtr<TypeScope> scope, Callback callback) {
	RefPtr<TypeScope> backup = m_current_scope;
	m_current_scope = scope;
	callback();
	m_current_scope = backup;
}

template <typename Callback>
RefPtr<TypeScope> TypeChecker::execute_on_new_scope(Callback callback) {
	RefPtr<TypeScope> new_scope = mk_ref<TypeScope>(m_current_scope);
	execute_on_scope(new_scope, callback);
	return new_scope;
}
