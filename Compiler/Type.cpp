#include "Type.hpp"

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
