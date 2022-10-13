#pragma once

#include <map>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "Aliases.hpp"
#include "Error.hpp"

namespace Kyra {

using declid_t = unsigned long;

template <typename T>
struct Element {
	declid_t declid;
	RefPtr<T> type;
};

class FunctionType;
class AppliedType;
// TODO: inherit private from Scope and make necessary functions available with using
class DeclaredType {
public:
	enum Kind { Integer, Function, Struct };

	explicit DeclaredType(std::string_view name, Kind kind);
	virtual ~DeclaredType() = default;

	bool can_be_assigned_to(const DeclaredType& other) const;
	bool insert_symbol_if_non_exists(std::string_view name, const Element<AppliedType>& element);
	std::optional<Element<AppliedType>> find_symbol(std::string_view name) const;
	std::vector<std::pair<std::string_view, Element<AppliedType>>> get_symbols() const;
	std::vector<Element<FunctionType>> find_methods(std::string_view name) const;
	bool insert_method_if_non_exists(std::string_view name, const Element<FunctionType>& element);

	std::string_view get_name() const;
	Kind get_kind() const;

protected:
	const std::string_view m_name;
	const Kind m_kind;
	std::map<std::string_view, Element<AppliedType>> m_symbols;
	std::map<std::string_view, std::vector<Element<FunctionType>>> m_methods;
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
	// TODO: remove name
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
	IntType(std::string_view name, unsigned width);

	unsigned get_width() const;

private:
	const unsigned m_width;
};

class StructType : public DeclaredType {
public:
	StructType(
		std::string_view name, const std::vector<std::pair<std::string_view, Element<AppliedType>>>& declarations);
};

class DeclarationDumpster {
public:
	struct Element {
		const std::string_view name;
		RefPtr<AppliedType> type;
	};

	static DeclarationDumpster& the() {
		static DeclarationDumpster instance;
		return instance;
	}

	DeclarationDumpster() = default;
	DeclarationDumpster(const DeclarationDumpster&) = delete;
	DeclarationDumpster(DeclarationDumpster&&) noexcept = default;

	DeclarationDumpster& operator=(const DeclarationDumpster&) = delete;
	DeclarationDumpster& operator=(DeclarationDumpster&&) noexcept = default;

	template <typename Callback>
	void abort_on_exception(const Callback& callback) {
		try {
			callback();
			commit_transaction();
		} catch(const ErrorException& e) {
			abort_transaction();
			throw e;
		}
	}

	declid_t insert(const Element& element);
	const Element& retrieve(declid_t id) const;

private:
	std::map<declid_t, Element> m_dumpster;
	std::map<declid_t, Element> m_transaction;

	void commit_transaction();
	void abort_transaction();
};

class TypeScope {
public:
	explicit TypeScope(RefPtr<TypeScope> parent = nullptr);

	std::optional<Element<AppliedType>> find_symbol(std::string_view name) const;
	bool insert_symbol(std::string_view name, Element<AppliedType> element);
	RefPtr<DeclaredType> find_type(std::string_view name) const;
	bool insert_type(std::string_view name, RefPtr<DeclaredType> type);
	std::vector<Element<FunctionType>> find_functions(std::string_view name) const;
	bool insert_function(std::string_view name, const Element<FunctionType>& element);

private:
	std::map<std::string_view, Element<AppliedType>> m_symbol_scope;
	std::map<std::string_view, RefPtr<DeclaredType>> m_type_scope;
	std::map<std::string_view, std::vector<Element<FunctionType>>> m_function_scope;
	RefPtr<TypeScope> m_parent;
};
}
