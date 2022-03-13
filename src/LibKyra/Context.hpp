#pragma once

#include <optional>
#include <unordered_map>

#include "HasPtrAlias.hpp"

namespace Kyra {
template <typename Class,
		typename VariableType,
		typename TypeType,
		typename TypeKeyType = std::string,
		typename VariableKeyType = std::string>
class Context : public HasPtrAlias<Class> {
public:
	explicit Context(std::shared_ptr<Class> parent = nullptr) : m_parent(parent) {}
	virtual ~Context() = default;

	template <typename S = VariableKeyType, typename T = VariableType>
	bool declare_var(S&& name, T&& value) {
		if(m_variables.contains(name) || m_functions.contains(name))
			return false;
		m_variables.try_emplace(std::forward<S>(name), std::forward<T>(value));
		return true;
	}

	template <typename S = TypeKeyType, typename T = TypeType>
	bool declare_type(S&& name, T&& type) {
		if(m_types.contains(name))
			return false;
		m_types.try_emplace(std::forward<S>(name), std::forward<T>(type));
		return true;
	}

	template <typename S = VariableKeyType, typename T = VariableType>
	bool declare_function(S&& name, T&& value) {
		if(m_functions.contains(name) || m_variables.contains(name))
			return false;
		m_functions.template try_emplace(std::forward<S>(name), std::forward<T>(value));
		return true;
	}

	virtual std::optional<VariableType> get_var(const VariableKeyType& name) const {
		if(const auto& it = m_variables.find(name); it != m_variables.end())
			return it->second;
		if(m_parent != nullptr)
			return m_parent->get_var(name);
		return {};
	}

	virtual std::optional<TypeType> get_type(const TypeKeyType& name) const {
		if(const auto& it = m_types.find(name); it != m_types.end())
			return it->second;
		if(m_parent != nullptr)
			return m_parent->get_type(name);
		return {};
	}

	virtual std::optional<VariableType> get_function(const VariableKeyType& name) const {
		if(const auto& it = m_functions.find(name); it != m_functions.end())
			return it->second;
		if(m_parent != nullptr)
			return m_parent->get_function(name);
		return {};
	}

	virtual bool remove_var(const VariableKeyType& name) {
		const auto& it = m_variables.find(name);
		if(it == m_variables.end())
			return false;
		m_variables.erase(it);
		return true;
	}

	virtual bool remove_function(const VariableKeyType& name) {
		const auto& it = m_functions.find(name);
		if(it == m_functions.end())
			return false;
		m_functions.erase(it);
		return true;
	}

	template <typename T = VariableType>
	bool mutate_var(const VariableKeyType& name, T&& value) {
		if(const auto& it = m_variables.find(name); it != m_variables.end()) {
			it->second = std::forward<T>(value);
			return true;
		}
		if(m_parent != nullptr)
			return m_parent->mutate_var(name, std::forward<T>(value));
		return false;
	}

	const std::unordered_map<VariableKeyType, VariableType>& get_variables() const { return m_variables; }
	const std::unordered_map<VariableKeyType, VariableType>& get_functions() const { return m_functions; };
	const std::unordered_map<TypeKeyType, TypeType>& get_types() const { return m_types; }

protected:
	std::shared_ptr<Class> m_parent;
	std::unordered_map<VariableKeyType, VariableType> m_variables;
	std::unordered_map<VariableKeyType, VariableType> m_functions;
	std::unordered_map<TypeKeyType, TypeType> m_types;
};
}