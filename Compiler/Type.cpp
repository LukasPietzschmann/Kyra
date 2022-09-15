#include "Type.hpp"

#include <utility>

namespace Kyra {

DeclaredType::DeclaredType(std::string_view name, Kind kind) : m_name(name), m_kind(kind) {}

bool DeclaredType::can_be_assigned_to(const DeclaredType& other) const { return m_name == other.m_name; }

std::string_view DeclaredType::get_name() const { return m_name; }

DeclaredType::Kind DeclaredType::get_kind() const { return m_kind; }

AppliedType::AppliedType(RefPtr<DeclaredType> decl_type, bool is_mutable) :
	m_decl_type(std::move(decl_type)), m_is_multable(is_mutable) {}

bool AppliedType::can_be_assigned_to(const AppliedType& other) const {
	if(!m_decl_type->can_be_assigned_to(*other.m_decl_type))
		return false;
	// var can be assigned to val and var
	// val can only be assigned to val
	return !(!m_is_multable && other.m_is_multable);
}

std::vector<RefPtr<FunctionType>> DeclaredType::find_methods(std::string_view name) const {
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
	methods.push_back(std::move(type));
}

const DeclaredType& AppliedType::get_declared_type() const { return *m_decl_type; }

RefPtr<DeclaredType> AppliedType::get_declared_type_shared() const { return m_decl_type; }

bool AppliedType::is_mutable() const { return m_is_multable; }

FunctionType::FunctionType(
	std::string_view name, RefPtr<DeclaredType> return_type, const std::vector<RefPtr<AppliedType>>& parameters) :
	DeclaredType(name, DeclaredType::Function),
	m_return_type(std::move(return_type)), m_parameters(parameters) {}

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

IntType::IntType(std::string_view name, unsigned width) : DeclaredType(name, DeclaredType::Integer), m_width(width) {}

unsigned IntType::get_width() const { return m_width; }

declid_t DeclarationDumpster::insert(const DeclarationDumpster::Element& element) {
	static declid_t id = 0;
	m_transaction.try_emplace(++id, element);
	return id;
}

const DeclarationDumpster::Element& DeclarationDumpster::retrieve(declid_t id) const {
	assert(m_transaction.empty());
	return m_dumpster.at(id);
}

void DeclarationDumpster::commit_transaction() {
	m_dumpster.insert(m_transaction.begin(), m_transaction.end());
	m_transaction.clear();
}

void DeclarationDumpster::abort_transaction() { m_transaction.clear(); }

TypeScope::TypeScope(RefPtr<TypeScope> parent) : m_parent(std::move(parent)) {
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

std::optional<TypeScope::Element<AppliedType>> TypeScope::find_symbol(std::string_view name) const {
	if(const auto& it = m_symbol_scope.find(name); it != m_symbol_scope.end())
		return it->second;
	if(m_parent != nullptr)
		return m_parent->find_symbol(name);
	return {};
}

bool TypeScope::insert_symbol(std::string_view name, TypeScope::Element<AppliedType> element) {
	if(m_symbol_scope.contains(name))
		return false;
	m_symbol_scope.try_emplace(name, element);
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

std::vector<TypeScope::Element<FunctionType>> TypeScope::find_functions(std::string_view name) const {
	// TODO: find functions from all visible scopes
	if(const auto& it = m_function_scope.find(name); it != m_function_scope.end())
		return it->second;
	if(m_parent != nullptr)
		return m_parent->find_functions(name);
	return {};
}

bool TypeScope::insert_function(std::string_view name, const TypeScope::Element<FunctionType>& element) {
	std::vector<TypeScope::Element<FunctionType>>& functions = m_function_scope[name];
	for(const auto& [id, function] : functions) {
		if(*function == *element.type)
			return false;
	}
	functions.push_back(element);
	return true;
}
}
