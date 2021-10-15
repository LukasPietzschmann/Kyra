#include "TypeContext.hpp"

namespace Slanguage {
bool TypeContext::setVar(const std::string& name, const Variable<Type::Ptr>& var) {
	if(m_variables.contains(name))
		return false;
	m_variables.try_emplace(name, var);
	return true;
}

bool TypeContext::setVar(const std::string& name, const Type::Ptr& varType, bool isMutable) {
	return setVar(name, Variable(varType, isMutable));
}

bool TypeContext::setType(const std::string& name, const Type::Ptr& type) {
	if(m_types.contains(name))
		return false;
	m_types.try_emplace(name, type);
	return true;
}

std::optional<Variable<Type::Ptr>> TypeContext::getVar(const std::string& name) const {
	if(const auto& it = m_variables.find(name); it != m_variables.end())
		return it->second;
	if(m_parent != nullptr)
		return m_parent->getVar(name);
	return {};
}

std::optional<Type::Ptr> TypeContext::getType(const std::string& name) const {
	if(const auto& it = m_types.find(name); it != m_types.end())
		return it->second;
	if(m_parent != nullptr)
		return m_parent->getType(name);
	return {};
}
}