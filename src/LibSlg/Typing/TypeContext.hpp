#pragma once
#include <iosfwd>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../HasPtrAlias.hpp"
#include "../Values/Value.hpp"
#include "../Variable.hpp"
#include "NativeTypes.hpp"
#include "Type.hpp"

namespace Slanguage {
class TypeContext : public HasPtrAlias<TypeContext> {
public:
	explicit TypeContext(TypeContext::Ptr parent = nullptr) : m_parent(parent) {
		for(const auto& nativeType : Value::NativeTypes::All)
			m_types.try_emplace(nativeType, NativeTypes::make(nativeType));
	}

	bool setType(const std::string& name, Type::Ptr type);
	bool setVar(const std::string& name, Type::Ptr varType, bool isMutable);
	bool setVar(const std::string& name, const Variable<Type::Ptr>& var);
	bool removeVar(const std::string& name);
	bool removeType(const std::string& name);
	std::optional<Type::Ptr> getType(const std::string& name) const;
	std::optional<Variable<Type::Ptr>> getVar(const std::string& name) const;
	const std::unordered_map<std::string, Variable<Type::Ptr>>& getVariables() const { return m_variables; }
	const std::unordered_map<std::string, Type::Ptr>& getTypes() const { return m_types; }

private:
	TypeContext::Ptr m_parent;
	std::unordered_map<std::string, Variable<Type::Ptr>> m_variables{};
	std::unordered_map<std::string, Type::Ptr> m_types{};
};
}