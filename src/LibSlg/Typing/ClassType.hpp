#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Type.hpp"

namespace Slanguage {
class ClassType : public Type {
public:
	ClassType(const std::string& name,
			std::unordered_map<std::string, Variable<Type::Ptr>> declarations = {},
			std::vector<Type::Ptr> constructorParams = {}) :
		Type(name),
		m_declarations(std::move(declarations)), m_constructorParameter(std::move(constructorParams)) {}
	~ClassType() override = default;

	std::optional<Variable<Type::Ptr>> knowsAbout(const std::string& name) const override {
		auto it = m_declarations.find(name);
		if(it == m_declarations.end())
			return {};
		return it->second;
	}

	const std::vector<Type::Ptr>& getConstructorParameter() const { return m_constructorParameter; }
	unsigned long getArity() const { return m_constructorParameter.size(); }

private:
	std::unordered_map<std::string, Variable<Type::Ptr>> m_declarations;
	std::vector<Type::Ptr> m_constructorParameter;
};
}