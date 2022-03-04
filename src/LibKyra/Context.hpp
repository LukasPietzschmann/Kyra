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

	virtual bool declare_var(const std::string& name, VT&& value) {
		if(m_variables.contains(name))
			return false;
		m_variables.try_emplace(name, std::forward<VT>(value));
		return true;
	}

	virtual bool declare_type(const std::string& name, TT&& type) {
		if(m_types.contains(name))
			return false;
		m_types.try_emplace(name, std::forward<TT>(type));
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

	virtual bool mutate_var(const std::string& name, VT&& value) {
		if(const auto& it = m_variables.find(name); it != m_variables.end()) {
			it->second = std::forward<VT>(value);
			return true;
		}
		if(m_parent != nullptr) {
			m_parent->mutate_var(name, std::forward<VT>(value));
			return true;
		}
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