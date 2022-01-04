#include "ClassType.hpp"

namespace Kyra {
std::optional<Variable<Type::Repr>> ClassType::knowsAbout(const std::string& name) const {
	auto it = m_declarations.find(name);
	if(it == m_declarations.end())
		return {};
	return it->second;
}

void ClassType::addConstructorParam(const Type::Repr& param) { m_constructorParameter.push_back(param); }

void ClassType::addDeclaration(const std::string& name, const Variable<Type::Repr>& value) {
	m_declarations.try_emplace(name, value);
}

const std::vector<Type::Repr>& ClassType::getConstructorParameter() const { return m_constructorParameter; }

unsigned long ClassType::getArity() const { return m_constructorParameter.size(); }
}