#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Type.hpp"

namespace LibSlg {
class ClassType : public Type {
public:
	ClassType(const std::string& name, std::unordered_map<std::string, Variable> declarations = {},
			std::vector<Type::Ptr> constructorParams = {}) :
		Type(name),
		m_declarations(std::move(declarations)), m_constructorParameter(std::move(constructorParams)) {}
	~ClassType() override = default;

	std::optional<Variable> knowsAbout(const std::string& name) const override {
		auto it = m_declarations.find(name);
		if(it == m_declarations.end())
			return {};
		return it->second;
	}

	Type::Ptr duplicate() const override { return Type::makePtr<ClassType>(*this); }

	const std::vector<Type::Ptr>& getConstructorParameter() const { return m_constructorParameter; }
	unsigned int getArity() const { return m_constructorParameter.size(); }

protected:
	std::unordered_map<std::string, Variable> m_declarations;
	std::vector<Type::Ptr> m_constructorParameter;
};
}