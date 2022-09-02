#pragma once

#include <map>
#include <string_view>
#include <vector>

#include "Aliases.hpp"

class FunctionType;
class DeclaredType {
public:
	explicit DeclaredType(std::string_view name);
	virtual ~DeclaredType() = default;

	bool can_be_assigned_to(const DeclaredType& other) const;
	const std::vector<RefPtr<FunctionType>> find_methods(std::string_view name) const;
	void insert_method_if_non_exists(std::string_view name, RefPtr<FunctionType> type);

	const std::string_view get_name() const;

protected:
	const std::string_view m_name;
	std::map<std::string_view, std::vector<RefPtr<FunctionType>>> m_methods;
};

class AppliedType final {
public:
	AppliedType(RefPtr<DeclaredType> decl_type, bool is_mutable);

	static RefPtr<AppliedType> promote_declared_type(RefPtr<DeclaredType> declared_type, bool is_mutable) {
		return mk_ref<AppliedType>(declared_type, is_mutable);
	}

	bool can_be_assigned_to(const AppliedType& other) const;

	const DeclaredType& get_declared_type() const;
	RefPtr<DeclaredType> get_declared_type_shared() const;
	bool is_mutable() const;

private:
	RefPtr<DeclaredType> m_decl_type;
	const bool m_is_multable;
};

class FunctionType : public DeclaredType {
public:
	FunctionType(
		std::string_view name, RefPtr<DeclaredType> return_type, const std::vector<RefPtr<AppliedType>>& parameters);

	RefPtr<DeclaredType> get_returned_type() const;
	const std::vector<RefPtr<AppliedType>>& get_parameter() const;

	bool can_be_called_with(const std::vector<RefPtr<AppliedType>>& arguments) const;

	bool operator==(const FunctionType& other) const;

private:
	RefPtr<DeclaredType> m_return_type;
	const std::vector<RefPtr<AppliedType>> m_parameters;
};

class IntType : public DeclaredType {
public:
	explicit IntType(std::string_view name, unsigned width);

	unsigned get_width() const;

private:
	const unsigned m_width;
};

class TypeScope {
public:
	explicit TypeScope(RefPtr<TypeScope> parent = nullptr);

	RefPtr<AppliedType> find_symbol(std::string_view name) const;
	bool insert_symbol(std::string_view name, RefPtr<AppliedType> type);
	RefPtr<DeclaredType> find_type(std::string_view name) const;
	bool insert_type(std::string_view name, RefPtr<DeclaredType> type);
	const std::vector<RefPtr<FunctionType>> find_functions(std::string_view name) const;
	bool insert_function(std::string_view name, RefPtr<FunctionType> type);

private:
	std::map<std::string_view, RefPtr<AppliedType>> m_symbol_scope;
	std::map<std::string_view, RefPtr<DeclaredType>> m_type_scope;
	std::map<std::string_view, std::vector<RefPtr<FunctionType>>> m_function_scope;
	RefPtr<TypeScope> m_parent;
};
