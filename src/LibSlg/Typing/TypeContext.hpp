#pragma once
#include <optional>
#include <string>
#include <unordered_map>

#include "../Values/Value.hpp"
#include "NativeTypes.hpp"
#include "Type.hpp"

namespace Slanguage {
class TypeContext {
public:
	explicit TypeContext(TypeContext* parent) : m_parent(parent) {
		for(const auto& nativeType : Value::NativeTypes::All)
			m_types.try_emplace(nativeType, NativeTypes::make(nativeType));
	}
	~TypeContext() { delete m_parent; }

	TypeContext(const TypeContext& other) :
		m_parent(new TypeContext(*other.m_parent)), m_variables(other.m_variables), m_types(other.m_types) {}

	TypeContext(TypeContext&& other) noexcept :
		m_parent(other.m_parent), m_variables(std::move(other.m_variables)), m_types(std::move(other.m_types)) {
		other.m_parent = nullptr;
	}

	TypeContext& operator=(const TypeContext& other) {
		if(this == &other)
			return *this;
		m_parent = new TypeContext(*other.m_parent);
		m_variables = other.m_variables;
		m_types = other.m_types;
		return *this;
	}

	TypeContext& operator=(TypeContext&& other) noexcept {
		if(this == &other)
			return *this;
		m_parent = other.m_parent;
		other.m_parent = nullptr;
		m_variables = std::move(other.m_variables);
		m_types = std::move(other.m_types);
		return *this;
	}

	bool setType(const std::string& name, const Type::Ptr& type);
	bool setVar(const std::string& name, const Type::Ptr& varType, bool isMutable);
	bool setVar(const std::string& name, const Variable<Type::Ptr>& var);
	std::optional<Type::Ptr> getType(const std::string& name) const;
	std::optional<Variable<Type::Ptr>> getVar(const std::string& name) const;
	const std::unordered_map<std::string, Variable<Type::Ptr>>& getVariables() const { return m_variables; }
	const std::unordered_map<std::string, Type::Ptr>& getTypes() const { return m_types; }

private:
	TypeContext* m_parent;
	std::unordered_map<std::string, Variable<Type::Ptr>> m_variables{};
	std::unordered_map<std::string, Type::Ptr> m_types{};
};
}