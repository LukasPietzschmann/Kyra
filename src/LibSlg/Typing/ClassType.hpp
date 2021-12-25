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
			std::unordered_map<std::string, Variable<Type::Repr>> declarations = {},
			std::vector<Type::Repr> constructorParams = {}) :
		Type(name),
		m_declarations(std::move(declarations)), m_constructorParameter(std::move(constructorParams)) {}
	~ClassType() override = default;

	std::optional<Variable<Type::Repr>> knowsAbout(const std::string& name) const override {
		auto it = m_declarations.find(name);
		if(it == m_declarations.end())
			return {};
		return it->second;
	}

	void addConstructorParam(const Type::Repr& param) { m_constructorParameter.push_back(param); }

	void addDeclaration(const std::string& name, const Variable<Type::Repr>& value) {
		m_declarations.emplace(name, value);
	}

	const std::vector<Type::Repr>& getConstructorParameter() const { return m_constructorParameter; }
	unsigned long getArity() const { return m_constructorParameter.size(); }

private:
	std::unordered_map<std::string, Variable<Type::Repr>> m_declarations;
	std::vector<Type::Repr> m_constructorParameter;
};
}