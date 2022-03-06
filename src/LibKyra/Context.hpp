#pragma once

#include <optional>
#include <unordered_map>

#include "HasPtrAlias.hpp"

namespace Kyra {
template <typename C, typename VT, typename TT>
class Context : public HasPtrAlias<C> {
public:
	explicit Context(std::shared_ptr<C> parent = nullptr) : m_parent(parent) {}
	virtual ~Context() = default;

	template <typename S = std::string, typename T = VT>
	bool declare_var(S&& name, T&& value) {
		if(m_variables.contains(name))
			return false;
		m_variables.try_emplace(std::forward<S>(name), std::forward<T>(value));
		return true;
	}

	template <typename S = std::string, typename T = TT>
	bool declare_type(S&& name, T&& type) {
		if(m_types.contains(name))
			return false;
		m_types.try_emplace(std::forward<S>(name), std::forward<T>(type));
		return true;
	}

	virtual std::optional<VT> get_var(const std::string& name) const {
		if(const auto& it = m_variables.find(name); it != m_variables.end())
			return it->second;
		if(m_parent != nullptr)
			return m_parent->get_var(name);
		return {};
	}

	virtual std::optional<TT> get_type(const std::string& name) const {
		if(const auto& it = m_types.find(name); it != m_types.end())
			return it->second;
		if(m_parent != nullptr)
			return m_parent->get_type(name);
		return {};
	}

	virtual bool remove_var(const std::string& name) {
		const auto& it = m_variables.find(name);
		if(it == m_variables.end())
			return false;
		m_variables.erase(it);
		return true;
	}

	template <typename T = VT>
	bool mutate_var(const std::string& name, T&& value) {
		if(const auto& it = m_variables.find(name); it != m_variables.end()) {
			it->second = std::forward<T>(value);
			return true;
		}
		if(m_parent != nullptr)
			return m_parent->mutate_var(name, std::forward<T>(value));
		return false;
	}

	const std::unordered_map<std::string, VT>& get_variables() const { return m_variables; }
	const std::unordered_map<std::string, TT>& get_types() const { return m_types; }

protected:
	std::shared_ptr<C> m_parent;
	std::unordered_map<std::string, VT> m_variables;
	std::unordered_map<std::string, TT> m_types;
};
}